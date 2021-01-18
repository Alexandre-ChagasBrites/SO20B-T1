#include "descritor_processo.h"

#include <iostream>

DescritorProcesso::DescritorProcesso(DescritorJob *job, unsigned int tempo) :
    job(job), memoria(job->memoriaNecessaria), cpuEstado(), dispositivos(), estado(DescritorProcesso::Estado::Bloqueado),
    quantum(1), prioridade(0.5f), horaInicio(tempo), horaTermino(0), tempoRetorno(0), tempoCPU(0),
    cpuUtilizada(0.0f), tempoBloqueado(0), numBloqueou(0), numEscalonado(0), numPreempcao(0)
{
    for (unsigned int i = 0; i < job->dispositivos.size(); i++)
        dispositivos.push_back(DispositivoES(job->dispositivos[i].nome));
}

void DescritorProcesso::Bloquear()
{
    estado = Estado::Bloqueado;
    numBloqueou++;
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

std::ostream &operator<<(std::ostream &os, const DescritorProcesso &processo)
{
    int porcentagem = (int)(processo.cpuUtilizada * 100.0f + 0.5f);
    os << " Hora de inicio: " << processo.horaInicio << std::endl;
    os << " Hora de termino: " << processo.horaTermino << std::endl;
    os << " Tempo de retorno: " << processo.tempoRetorno << std::endl;
    os << " Tempo de CPU: " << processo.tempoCPU << std::endl;
    os << " Percentual de CPU utilizada: " << porcentagem << std::endl;
    os << " Tempo bloqueado: " << processo.tempoBloqueado << std::endl;
    os << " Numero de vezes que bloqueou: " << processo.numBloqueou << std::endl;
    os << " Numero de vezes que foi escalonado: " << processo.numEscalonado << std::endl;
    os << " Numero de vezes que perdeu a CPU por preempcao: " << processo.numPreempcao << std::endl;
    return os;
}
