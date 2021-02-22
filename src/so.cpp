#include "so.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>

SO::SO(unsigned int tamanhoPagina, unsigned int tamanhoTotal, TrocaDePagina trocaDePagina, unsigned int quantum, Prioridade prioridade) :
    memoriaFisica(tamanhoPagina, tamanhoTotal), mmu(memoriaFisica), cpu(mmu),
    controlador(), temporizador(), escalonador(),
    mapa(tamanhoTotal), quadrosFila(tamanhoTotal), jobs(), processos(), processoAtual(-1), processosExecutando(0),
    trocaDePagina(trocaDePagina), quantum(quantum), prioridade(prioridade)
{
    for (unsigned int i = 0; i < tamanhoTotal; i++)
        quadrosFila[i] = i;
    temporizador.PedirInterrupcao(true, 10, "ESCALONAR");
}

SO::~SO()
{
    for (DescritorJob *job : jobs)
        delete job;
    for (DescritorProcesso *processo : processos)
        delete processo;
}

void SO::Executa()
{
    if (processosExecutando == 0)
        return;
    Escalonar();
    controlador.Executa(this, &cpu, &temporizador);
    ImprimirRelatorio();
}

void SO::CarregarPrograma(std::string arquivo, unsigned int tempo)
{
    DescritorJob *job = new DescritorJob(arquivo, temporizador.ObterTempo());
    jobs.push_back(job);

    processos.push_back(new DescritorProcesso(job, tempo, memoriaFisica));
    processosExecutando++;

    temporizador.PedirInterrupcao(false, tempo, "APRONTAR " + std::to_string(processos.size() - 1));
}

bool SO::InstrucaoIlegal()
{
    ContabilizarTempo();
    bool continuar = true;
    //std::cout << "INSTRUCAO ILEGAL\n";
    std::stringstream instrucao(cpu.Instrucao());
    std::string comando;
    instrucao >> comando;

    if (comando == "PARA")
    {
        //PARA  	pede ao SO para terminar a execuчуo do programa (como exit)

        EncerrarProcesso();
        continuar = processosExecutando > 0;
    }
    else if (comando == "LE")
    {
        //LE	n	pede ao SO para fazer a leitura de um dado (inteiro) do dispositivo de E/S n;
        //          o dado sera colocado no acumulador
        int n;
        instrucao >> n;

        processos[processoAtual]->Bloquear();
        cpu.SalvaEstado(processos[processoAtual]->cpuEstado);

        int valor = processos[processoAtual]->dispositivos[n].Ler();
        processos[processoAtual]->cpuEstado.AlteraAcumulador(valor);

        unsigned int periodo = processos[processoAtual]->job->dispositivos[n].tempoLeitura;
        temporizador.PedirInterrupcao(false, periodo, "APRONTAR " + std::to_string(processoAtual));
    }
    else if (comando == "GRAVA")
    {
        //GRAVA n	pede ao SO gravar o valor do acumulador no dispositivo de E/S n
        int n;
        instrucao >> n;

        processos[processoAtual]->Bloquear();
        cpu.SalvaEstado(processos[processoAtual]->cpuEstado);

        int valor = processos[processoAtual]->cpuEstado.ObterAcumulador();
        processos[processoAtual]->dispositivos[n].Gravar(valor);

        unsigned int periodo = processos[processoAtual]->job->dispositivos[n].tempoGravacao;
        temporizador.PedirInterrupcao(false, periodo, "APRONTAR " + std::to_string(processoAtual));
    }

    Escalonar();
    qntInstrucaoIlegal++;
    return continuar;
}

bool SO::ViolacaoDeMemoria()
{
    ContabilizarTempo();
    bool continuar = true;
    //std::cout << "VIOLACAO DE MEMORIA\n";
    if (mmu.ObterErro() == MMU::Erro::PaginaInvalida ||
        mmu.ObterErro() == MMU::Erro::AcessoInvalido)
        numFalhaPagina++;

    if (mmu.ObterErro() == MMU::Erro::PaginaInvalida)
    {
        processos[processoAtual]->BloquearPorFaltaDePagina();
        cpu.SalvaEstado(processos[processoAtual]->cpuEstado);
        AlterarTabela(processoAtual, mmu.ObterPagina());
    }
    else 
    {
        EncerrarProcesso();
        continuar = processosExecutando > 0;
    }

    Escalonar();
    qntViolacaoDeMemoria++;
    return continuar;
}

void SO::InterrupcaoTemporizador(std::string interrupcao)
{
    ContabilizarTempo();
    //std::cout << "INTERRUPCAO " << interrupcao << "\n";
    std::stringstream instrucao(interrupcao);
    std::string comando;
    instrucao >> comando;

    if (comando == "APRONTAR")
    {
        unsigned int n;
        instrucao >> n;

        processos[n]->Aprontar();
    }
    else if (comando == "ALTERAR_TABELA")
    {
        unsigned int processo;
        unsigned int pagina;

        instrucao >> processo;
        instrucao >> pagina;

        AlterarTabela(processo, pagina);
    }
    else if (comando == "ALTERAR_QUADRO")
    {
        unsigned int quadro;
        unsigned int processo;
        unsigned int pagina;

        instrucao >> quadro;
        instrucao >> processo;
        instrucao >> pagina;

        AlterarQuadro(quadro, processo, pagina);
    }
    else if (comando == "ALVO")
    {
        unsigned int quadro;
        instrucao >> quadro;

        mapa[quadro].alvo = false;
    }

    Escalonar();
    qntInterrupcaoTemporizador++;
}

void SO::AlterarTabela(unsigned int processo, unsigned int pagina)
{
    unsigned int quadro = ProximoQuadro();
    //std::cout << "ALTERAR TABELA " << quadro << " PARA " << processo << " " << pagina << "\n";
    if (!mapa[quadro].alvo)
    {
        bool duasOperacoes = false;
        if (!mapa[quadro].livre)
        {
            unsigned int processoAnterior = mapa[quadro].processo;
            unsigned int paginaAnterior = mapa[quadro].pagina;
            //std::cout << "PUTS OCUPADO " << processoAnterior << " " << paginaAnterior << "\n";
            MMU::DescritorPagina &descritorPagina = processos[processoAnterior]->tabela[paginaAnterior];
            descritorPagina.valida = false;

            if (descritorPagina.alterada)
            {
                duasOperacoes = true;
                mapa[quadro].alvo = true;

                Memoria::Quadro &valor = memoriaFisica.LerQuadro(quadro);
                processos[processoAnterior]->memoriaSecundaria.AlterarQuadro(paginaAnterior, valor);
                
                std::string parametros = std::to_string(quadro) + " " + std::to_string(processo) + " " + std::to_string(pagina);
                temporizador.PedirInterrupcao(false, 10, "ALTERAR_QUADRO " + parametros);
            }
        }

        if (!duasOperacoes)
            AlterarQuadro(quadro, processo, pagina);
    }
    else
    {
        std::string parametros = std::to_string(processo) + " " + std::to_string(pagina);
        temporizador.PedirInterrupcao(false, 10, "ALTERAR_TABELA " + parametros);
    }
}

void SO::AlterarQuadro(unsigned int quadro, unsigned int processo, unsigned int pagina)
{
    mapa[quadro].livre = false;
    mapa[quadro].processo = processo;
    mapa[quadro].pagina = pagina;
    mapa[quadro].alvo = true;

    Memoria::Quadro &valor = processos[processo]->memoriaSecundaria.LerQuadro(pagina);
    memoriaFisica.AlterarQuadro(quadro, valor);

    MMU::DescritorPagina &descritorPagina = processos[processo]->tabela[pagina];
    descritorPagina.valida = true;
    descritorPagina.quadro = quadro;
    descritorPagina.acessada = false;
    descritorPagina.alterada = false;
    //std::cout << "ALTERAR QUADRO " << quadro << " PARA " << processo << " " << pagina << "\n";
    temporizador.PedirInterrupcao(false, 10, "APRONTAR " + std::to_string(processo));
    temporizador.PedirInterrupcao(false, 10 + quantum, "ALVO " + std::to_string(quadro));
}

unsigned int SO::ProximoQuadro()
{
    unsigned int quadro = quadrosFila.front();
    quadrosFila.pop_front();
    if (trocaDePagina == TrocaDePagina::SegundaChance)
    {
        if (!mapa[quadro].livre)
        {
            MMU::DescritorPagina &descritorPagina = processos[mapa[quadro].processo]->tabela[mapa[quadro].pagina];
            if (descritorPagina.acessada)
            {
                descritorPagina.acessada = false;
                quadrosFila.push_back(quadro);

                quadro = quadrosFila.front();
                quadrosFila.pop_front();
            }
        }
    }
    quadrosFila.push_back(quadro);
    return quadro;
}

void SO::EncerrarProcesso()
{
    for (unsigned int i = 0; i < processos[processoAtual]->tabela.size(); i++)
    {
        if (processos[processoAtual]->tabela[i].valida)
        {
            Memoria::Quadro &valor = memoriaFisica.LerQuadro(processos[processoAtual]->tabela[i].quadro);
            processos[processoAtual]->memoriaSecundaria.AlterarQuadro(i, valor);
        }
    }

    std::vector<unsigned int> quadrosRemovidos;

    auto i = quadrosFila.begin();
    while (i != quadrosFila.end())
    {
        unsigned int quadro = *i;
        DescritorQuadro &descritorQuadro = mapa[quadro];

        if (!descritorQuadro.livre && descritorQuadro.processo == processoAtual)
        {
            i = quadrosFila.erase(i);
            descritorQuadro.livre = true;
            quadrosRemovidos.push_back(quadro);
        }
        else
            i++;
    }

    for (unsigned int quadro : quadrosRemovidos)
        quadrosFila.push_front(quadro);

    //std::cout << "ENCERRAR " << processoAtual << "\n";
    processos[processoAtual]->Terminar(temporizador.ObterTempo());
    processoAtual = -1;
    processosExecutando--;
}

void SO::ContabilizarTempo()
{
    qntExecutado++;
    unsigned int tempo = temporizador.ObterTempo() - tempoAtivo;

    if (processoAtual != -1)
    {
        processos[processoAtual]->tempoCPU += tempo;
        processos[processoAtual]->quantum -= tempo;
        if (processos[processoAtual]->quantum < 0)
            processos[processoAtual]->quantum = 0;
    }
    else
        tempoOcioso += tempo;

    for (unsigned int i = 0; i < processos.size(); i++)
    {
        if (processos[i]->estado == DescritorProcesso::Estado::Terminado)
            continue;
        if (processos[i]->estado == DescritorProcesso::Estado::Bloqueado ||
            processos[i]->estado == DescritorProcesso::Estado::BloqueadoFaltaPagina)
            processos[i]->tempoBloqueado += tempo;
        if (processos[i]->estado == DescritorProcesso::Estado::Bloqueado)
            processos[i]->bloqueadoES += tempo;
        if (processos[i]->estado == DescritorProcesso::Estado::BloqueadoFaltaPagina)
            processos[i]->bloqueadoEsperaPagina += tempo;
        if (processos[i]->estado == DescritorProcesso::Estado::Pronto)
            processos[i]->tempoEsperaEscalonamento += tempo;
    }

    tempoAtivo = temporizador.ObterTempo();
}

void SO::Escalonar()
{
    int processo = escalonador.RetornaProcesso(processoAtual, &processos);

    if (processo != processoAtual)
    {
        if (processoAtual != -1 && processos[processoAtual]->estado == DescritorProcesso::Estado::Execucao)
        {
            processos[processoAtual]->Preempcao();
            switch (prioridade)
            {
            case Prioridade::Abaixo:
                processos[processoAtual]->prioridade = (processos[processoAtual]->prioridade + processos[processoAtual]->quantum / (float)quantum) * 0.5f;
                break;
            case Prioridade::Acima:
                processos[processoAtual]->prioridade = (processos[processoAtual]->prioridade + (quantum - processos[processoAtual]->quantum) / (float)quantum) * 0.5f;
                break;
            default:
                break;
            }
            cpu.SalvaEstado(processos[processoAtual]->cpuEstado);
            qntPreempcao++;
        }

        if (processo != -1)
        {
            processos[processo]->Executar();
            processos[processo]->quantum = quantum;

            mmu.UsarTabela(&processos[processo]->tabela);
            cpu.AlteraPrograma(&processos[processo]->job->programa);
            cpu.AlteraEstado(processos[processo]->cpuEstado);
            cpu.RetornaInterrupcao();

            qntTrocaProcesso++;
            temporizador.PedirInterrupcao(false, quantum, "ESCALONAR");
        }
    }

    if (processo == -1)
        cpu.Dormir();
    //std::cout << "ESCALONAR " << processoAtual << " PARA " << processo << "\n";
    processoAtual = processo;
}

void SO::ImprimirRelatorio()
{
    std::ofstream file("relatorio.txt");

    file << "SO:\n";
    file << " Tamanho de pagina: " << memoriaFisica.TamanhoQuadro() << "\n";
    file << " Tamanho total: " << memoriaFisica.TamanhoTotal() << "\n";
    file << " Troca de pagina: ";
    switch (trocaDePagina)
    {
    case TrocaDePagina::FIFO:          file << "FIFO\n"; break;
    case TrocaDePagina::SegundaChance: file << "Segunda chance\n"; break;
    }
    file << " Quantum: " << quantum << std::endl;
    file << " Prioridade: ";
    switch (prioridade)
    {
        case Prioridade::Fixa:   file << "Fixa\n";   break;
        case Prioridade::Abaixo: file << "Abaixo\n"; break;
        case Prioridade::Acima:  file << "Acima\n";  break;
    }
    file << std::endl;

    file << "Tempo em que o sistema esteve ativo: " << tempoAtivo << std::endl;
    file << "Tempo ocioso total da CPU: " << tempoOcioso << std::endl;
    file << "Numero total de falhas de pagina atendidas: " << numFalhaPagina << std::endl;
    file << "Quantas vezes o SO executou: " << qntExecutado << std::endl;
    file << " Quantas foram instrucoes ilegais: " << qntInstrucaoIlegal << std::endl;
    file << " Quantas foram violacoes de memoria: " << qntViolacaoDeMemoria << std::endl;
    file << " Quantas foram interrupcoes pelo temporizador: " << qntInterrupcaoTemporizador << std::endl;
    file << "Quantas trocas de processos houve: " << qntTrocaProcesso << std::endl;
    file << " Quantas foram preempcao: " << qntPreempcao << std::endl;
    file << std::endl;

    {
        float media = 0;
        for (unsigned int i = 0; i < processos.size(); i++)
            media += processos[i]->cpuUtilizada;
        media /= processos.size();

        float desvioPadrao = 0.0f;
        for (unsigned int i = 0; i < processos.size(); i++)
            desvioPadrao += (processos[i]->cpuUtilizada - media) * (processos[i]->cpuUtilizada - media);
        desvioPadrao = std::sqrt(desvioPadrao / processos.size());

        file << "CPU utilizada por processo:\n";
        file << " Media: " << (int)(media * 100.0f) << std::endl;
        file << " Desvio padrao: " << (desvioPadrao * 100.0f) << std::endl;
        file << " Coeficiente de variacao: " << (int)(desvioPadrao * 100.0f / media) << std::endl;
        file << std::endl;
    }

    for (unsigned int i = 0; i < processos.size(); i++)
    {
        file << "Processo " << i << "(" << processos[i]->job->arquivo << "):\n";
        file << *(processos[i]) << std::endl;
    }

    file.close();
}
