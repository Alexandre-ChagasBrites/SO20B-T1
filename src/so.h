#pragma once

#include <vector>
#include <string>
#include "cpu.h"
#include "controlador.h"
#include "temporizador.h"
#include "escalonador.h"
#include "descritor_job.h"
#include "descritor_processo.h"
#include "dispositivo_es.h"

class SO
{
public:
    enum class Prioridade
    {
        Fixa, Abaixo, Acima
    };

public:
    SO(unsigned int quantum, Prioridade prioridade);

    void Executa();
    void CarregarPrograma(std::string arquivo, unsigned int tempo);

    bool InstrucaoIlegal();
    bool ViolacaoDeMemoria();
    void InterrupcaoTemporizador(std::string interrupcao);

private:
    void ContabilizarTempo();
    void Escalonar();
    void ImprimirRelatorio();

private:
    CPU cpu;
    Controlador controlador;
    Temporizador temporizador;
    Escalonador escalonador;

    std::vector<DescritorJob*> jobs;
    std::vector<DescritorProcesso*> processos;
    DescritorProcesso *processoExecucao;
    unsigned int processosExecutando;
    unsigned int quantum;
    Prioridade prioridade;

    unsigned int tempoAtivo;
    unsigned int tempoOcioso;
    unsigned int qntExecutado;
    unsigned int qntInstrucaoIlegal;
    unsigned int qntViolacaoDeMemoria;
    unsigned int qntInterrupcaoTemporizador;
    unsigned int qntTrocaProcesso;
    unsigned int qntPreempcao;
};
