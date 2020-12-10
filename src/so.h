#pragma once

#include <vector>
#include "cpu.h"
#include "controlador.h"
#include "temporizador.h"
#include "dispositivo_es.h"
#include "descritor_job.h"

class SO
{
public:
    void Executa();

    void CarregarDispositivo(const char *arquivo);
    void CarregarJob(const char *arquivo);

    bool InstrucaoIlegal();
    bool ViolacaoDeMemoria();

public:
    CPU cpu;
    CPU::Estado estado;
    Controlador controlador;
    Temporizador temporizador;
    std::vector<DispositivoES> dispositivos;
    std::vector<DescritorJob> jobs;
};
