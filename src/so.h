#pragma once

#include "cpu.h"
#include "controlador.h"

class SO
{
public:
    void Inicializa(std::vector<std::string> *programa, std::vector<int> *dados);

    bool InstrucaoIlegal();
    bool ViolacaoDeMemoria();

public:
    CPU cpu;
    Controlador controlador;
};
