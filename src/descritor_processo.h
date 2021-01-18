#pragma once

#include <vector>
#include "cpu.h"
#include "descritor_job.h"
#include "dispositivo_es.h"

class DescritorProcesso
{
public:
    enum class Estado
    {
        Bloqueado, Pronto, Execucao, Terminado
    };

public:
    DescritorProcesso(DescritorJob *job, unsigned int tempo);

    void Bloquear();
    void Aprontar();
    void Executar();
    void Preempcao();
    void Terminar(unsigned int tempo);

    friend std::ostream &operator<<(std::ostream &os, const DescritorProcesso &processo);
    friend class SO;
    friend class Escalonador;

private:
    DescritorJob *job;
    std::vector<int> memoria;
    CPU::Estado cpuEstado;
    std::vector<DispositivoES> dispositivos;
    Estado estado;

    int quantum;
    float prioridade;

    unsigned int horaInicio;
    unsigned int horaTermino;
    unsigned int tempoRetorno;
    unsigned int tempoCPU;
    float cpuUtilizada;
    unsigned int tempoBloqueado;
    unsigned int numBloqueou;
    unsigned int numEscalonado;
    unsigned int numPreempcao;
};
