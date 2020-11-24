#pragma once

#include "cpu.h"
#include "controlador.h"
#include "temporizador.h"

class SO
{
public:
    void Inicializa(std::vector<std::string> *programa, std::vector<int> *dados);

    bool InstrucaoIlegal(Temporizador &temporizador);
    bool ViolacaoDeMemoria();

public:
    CPU cpu;
    CPU::Estado estado;
    Controlador controlador;
};
