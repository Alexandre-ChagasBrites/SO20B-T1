#pragma once

#include <vector>
#include "memoria.h"
#include "mmu.h"
#include "cpu.h"
#include "descritor_job.h"
#include "dispositivo_es.h"

class DescritorProcesso
{
public:
    enum class Estado
    {
        Bloqueado, BloqueadoFaltaPagina, Pronto, Execucao, Terminado
    };

public:
    DescritorProcesso(DescritorJob *job, unsigned int tempo, Memoria &memoriaFisica);

    void Bloquear();
    void BloquearPorFaltaDePagina();
    void Aprontar();
    void Executar();
    void Preempcao();
    void Terminar(unsigned int tempo);

    friend std::ostream &operator<<(std::ostream &os, DescritorProcesso &processo);
    friend class SO;
    friend class Escalonador;

private:
    DescritorJob *job;
    Memoria memoriaSecundaria;
    std::vector<MMU::DescritorPagina> tabela;
    CPU::Estado cpuEstado;
    std::vector<DispositivoES> dispositivos;

    Estado estado;
    int quantum;
    float prioridade;

    unsigned int horaInicio;
    unsigned int horaTermino = 0;
    unsigned int tempoRetorno = 0;
    unsigned int tempoCPU = 0;
    float cpuUtilizada = 0.0f;
    unsigned int tempoBloqueado = 0;
    unsigned int bloqueadoES = 0;
    unsigned int bloqueadoEsperaPagina = 0;
    unsigned int tempoEsperaEscalonamento = 0;
    unsigned int numBloqueou = 0;
    unsigned int numFalhaPagina = 0;
    unsigned int numEscalonado = 0;
    unsigned int numPreempcao = 0;
};
