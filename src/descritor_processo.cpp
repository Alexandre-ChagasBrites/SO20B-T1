#include "descritor_processo.h"

#include <iostream>

DescritorProcesso::DescritorProcesso(DescritorJob *job, unsigned int tempo, Memoria &memoriaFisica) :
    job(job), memoriaSecundaria(job->memoriaNecessaria, memoriaFisica), tabela(), cpuEstado(), dispositivos(),
    estado(DescritorProcesso::Estado::Terminado), quantum(1), prioridade(0.5f), horaInicio(tempo)
{
    tabela.resize(memoriaSecundaria.TamanhoTotal());
    for (unsigned int i = 0; i < job->dispositivos.size(); i++)
        dispositivos.push_back(DispositivoES(job->dispositivos[i].nome));
}

void DescritorProcesso::Bloquear()
{
    estado = Estado::Bloqueado;
    numBloqueou++;
}

void DescritorProcesso::BloquearPorFaltaDePagina()
{
    estado = Estado::BloqueadoFaltaPagina;
    numBloqueou++;
    numFalhaPagina++;
}

void DescritorProcesso::Aprontar()
{
    estado = Estado::Pronto;
}

void DescritorProcesso::Executar()
{
    estado = Estado::Execucao;
    numEscalonado++;
}

void DescritorProcesso::Preempcao()
{
    estado = Estado::Pronto;
    numPreempcao++;
}

void DescritorProcesso::Terminar(unsigned int tempo)
{
    estado = Estado::Terminado;
    horaTermino = tempo;
    tempoRetorno = tempo - job->dataLancamento;
    cpuUtilizada = (float)tempoCPU / (horaTermino - horaInicio);
}

std::ostream &operator<<(std::ostream &os, DescritorProcesso &processo)
{
    int porcentagem = (int)(processo.cpuUtilizada * 100.0f + 0.5f);
    os << " Hora de inicio: " << processo.horaInicio << std::endl;
    os << " Hora de termino: " << processo.horaTermino << std::endl;
    os << " Tempo de retorno: " << processo.tempoRetorno << std::endl;
    os << " Tempo de CPU: " << processo.tempoCPU << std::endl;
    os << " Percentual de CPU utilizada: " << porcentagem << std::endl;
    os << " Tempo bloqueado: " << processo.tempoBloqueado << std::endl;
    os << "  Tempo bloqueado em E/S: " << processo.bloqueadoES << std::endl;
    os << "  Tempo Bloqueado esperando troca de pagina: " << processo.bloqueadoEsperaPagina << std::endl;
    os << " Tempo de espera em fila de escalonamento: " << processo.tempoEsperaEscalonamento << std::endl;
    os << " Numero de falhas de pagina: " << processo.numFalhaPagina << std::endl;
    os << " Numero de vezes que bloqueou: " << processo.numBloqueou << std::endl;
    os << " Numero de vezes que foi escalonado: " << processo.numEscalonado << std::endl;
    os << " Numero de vezes que perdeu a CPU por preempcao: " << processo.numPreempcao << std::endl;
    os << " Quantidade de memoria: " << (processo.memoriaSecundaria.TamanhoQuadro() * processo.memoriaSecundaria.TamanhoTotal()) << std::endl;
    for (unsigned int i = 0; i < processo.memoriaSecundaria.TamanhoTotal(); i++)
    {
        os << "  ";
        for (unsigned int j = 0; j < processo.memoriaSecundaria.TamanhoQuadro(); j++)
        {
            unsigned int endereco = i * processo.memoriaSecundaria.TamanhoQuadro() + j;
            os << "0x" << std::hex << endereco << ": " << std::dec << processo.memoriaSecundaria.LerInteiro(endereco) << " | ";
        }
        os << "\n";
    }     
    return os;
}
