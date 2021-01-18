#include "so.h"

#include <iostream>
#include <fstream>
#include <cstring>
#include <cmath>

SO::SO(unsigned int quantum, Prioridade prioridade) :
    cpu(), controlador(), temporizador(), escalonador(),
    jobs(), processos(), processoExecucao(nullptr), processosExecutando(0),
    quantum(quantum), prioridade(prioridade),
    tempoAtivo(0), tempoOcioso(0), qntExecutado(0),
    qntInstrucaoIlegal(0), qntViolacaoDeMemoria(0), qntInterrupcaoTemporizador(0),
    qntTrocaProcesso(0), qntPreempcao(0)
{
    temporizador.PedirInterrupcao(true, 10, "ESCALONAR");
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

    processos.push_back(new DescritorProcesso(job, tempo));
    processosExecutando++;

    temporizador.PedirInterrupcao(false, tempo, "INICIAR PROCESSO");
}

bool SO::InstrucaoIlegal()
{
    ContabilizarTempo();
    bool continuar = true;

    std::string instrucao = cpu.Instrucao();
    const char *c_instrucao = instrucao.c_str();
    if (std::strstr(c_instrucao, "PARA"))
    {
        //PARA  	pede ao SO para terminar a execuчуo do programa (como exit)

        processoExecucao->Terminar(temporizador.ObterTempo());
        processoExecucao = nullptr;

        processosExecutando--;
        continuar = processosExecutando > 0;
    }
    else if (std::strstr(c_instrucao, "LE"))
    {
        //LE	n	pede ao SO para fazer a leitura de um dado (inteiro) do dispositivo de E/S n;
        //          o dado sera colocado no acumulador
        int n;
        std::sscanf(c_instrucao, "%*s %i", &n);

        processoExecucao->Bloquear();
        cpu.SalvaEstado(&processoExecucao->cpuEstado);
        processoExecucao->cpuEstado.AlteraAcumulador(processoExecucao->dispositivos[n].Ler());

        int index = 0;
        for (unsigned int i = 0; i < processos.size(); i++)
        {
            if (processos[i] == processoExecucao)
            {
                index = i;
                break;
            }
        }
        temporizador.PedirInterrupcao(false, processoExecucao->job->dispositivos[n].tempoLeitura, "RETORNA " + std::to_string(index));
    }
    else if (std::strstr(c_instrucao, "GRAVA"))
    {
        //GRAVA n	pede ao SO gravar o valor do acumulador no dispositivo de E/S n
        int n;
        std::sscanf(c_instrucao, "%*s %i", &n);

        processoExecucao->Bloquear();
        cpu.SalvaEstado(&processoExecucao->cpuEstado);
        processoExecucao->dispositivos[n].Gravar(processoExecucao->cpuEstado.ObterAcumulador());

        int index = 0;
        for (unsigned int i = 0; i < processos.size(); i++)
        {
            if (processos[i] == processoExecucao)
            {
                index = i;
                break;
            }
        }
        temporizador.PedirInterrupcao(false, processoExecucao->job->dispositivos[n].tempoGravacao, "RETORNA " + std::to_string(index));
    }

    Escalonar();
    qntInstrucaoIlegal++;
    return continuar;
}

bool SO::ViolacaoDeMemoria()
{
    ContabilizarTempo();

    std::cout << "Violacao de memoria" << std::endl;

    Escalonar();
    qntViolacaoDeMemoria++;
    return false;
}

void SO::InterrupcaoTemporizador(std::string interrupcao)
{
    ContabilizarTempo();

    const char *c_interrupcao = interrupcao.c_str();
    if (std::strstr(c_interrupcao, "RETORNA"))
    {
        unsigned int n;
        std::sscanf(c_interrupcao, "%*s %u", &n);

        processos[n]->Aprontar();
    }
    else if (std::strstr(c_interrupcao, "INICIAR PROCESSO"))
    {
        for (unsigned int i = 0; i < processos.size(); i++)
        {
            if (processos[i]->horaInicio == temporizador.ObterTempo())
                processos[i]->Aprontar();
        }
    }

    Escalonar();
    qntInterrupcaoTemporizador++;
}

 void SO::ContabilizarTempo()
 {
     qntExecutado++;
     unsigned int tempo = temporizador.ObterTempo() - tempoAtivo;

     if (processoExecucao != nullptr)
     {
        processoExecucao->tempoCPU += tempo;
        processoExecucao->quantum -= tempo;
        if (processoExecucao->quantum < 0)
            processoExecucao->quantum = 0;
     }
     else
        tempoOcioso += tempo;

     for (unsigned int i = 0; i < processos.size(); i++)
     {
         if (processos[i]->estado == DescritorProcesso::Estado::Bloqueado)
            processos[i]->tempoBloqueado += tempo;
     }

     tempoAtivo = temporizador.ObterTempo();
 }

void SO::Escalonar()
{
    DescritorProcesso *processo = escalonador.RetornaProcesso(processoExecucao, &processos);

    if (processo != processoExecucao)
    {
        if (processoExecucao != nullptr && processoExecucao->estado == DescritorProcesso::Estado::Execucao)
        {
            processoExecucao->Preempcao();
            switch (prioridade)
            {
            case Prioridade::Abaixo:
                processoExecucao->prioridade = (processoExecucao->prioridade + (quantum - processoExecucao->quantum) / (float)quantum) * 0.5f;
                break;
            case Prioridade::Acima:
                processoExecucao->prioridade = (processoExecucao->prioridade + processoExecucao->quantum / (float)quantum) * 0.5f;
                break;
            default:
                break;
            }
            cpu.SalvaEstado(&processoExecucao->cpuEstado);
            qntPreempcao++;
        }

        if (processo != nullptr)
        {
            processo->Executar();
            processo->quantum = quantum;
            cpu.AlteraPrograma(&processo->job->programa);
            cpu.AlteraDados(&processo->memoria);
            cpu.AlteraEstado(processo->cpuEstado);
            cpu.RetornaInterrupcao();
            qntTrocaProcesso++;
            temporizador.PedirInterrupcao(false, quantum, "ESCALONAR");
        }
    }

    if (processo == nullptr)
        cpu.Dormir();

    processoExecucao = processo;
}

void SO::ImprimirRelatorio()
{
    std::ofstream file("relatorio.txt");

    file << "SO:\n";
    file << " Quantum: " << quantum << std::endl;
    file << " Prioridade: ";
    switch (prioridade)
    {
        case Prioridade::Fixa:   file << "Fixa\n";   break;
        case Prioridade::Abaixo: file << "Abaixo\n"; break;
        case Prioridade::Acima:  file << "Acima\n";  break;
    }
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
        file << " Desvio padrao: " << (int)(desvioPadrao * 100.0f) << std::endl;
        file << " Coeficiente de variacao: " << (int)(desvioPadrao * 100.0f / media) << std::endl;
        file << std::endl;
    }

    file << "Tempo em que o sistema esteve ativo: " << tempoAtivo << std::endl;
    file << "Tempo ocioso total da CPU: " << tempoOcioso << std::endl;
    file << "Quantas vezes o SO executou: " << qntExecutado << std::endl;
    file << " Quantas foram instrucoes ilegais: " << qntInstrucaoIlegal << std::endl;
    file << " Quantas foram violacoes de memoria: " << qntViolacaoDeMemoria << std::endl;
    file << " Quantas foram interrupcoes pelo temporizador: " << qntInterrupcaoTemporizador << std::endl;
    file << "Quantas trocas de processos houve: " << qntTrocaProcesso << std::endl;
    file << " Quantas foram preempcao: " << qntPreempcao << std::endl;
    file << std::endl;

    for (unsigned int i = 0; i < processos.size(); i++)
    {
        file << "Processo " << processos[i]->job->arquivo << ":\n";
        file << *(processos[i]) << std::endl;
    }

    file.close();
}
