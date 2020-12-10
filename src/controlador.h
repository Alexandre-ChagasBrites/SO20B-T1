#pragma once

#include "temporizador.h"

class SO;
class CPU;

class Controlador
{
public:
    void Executa(SO *so, CPU *cpu, Temporizador *temporizador);
};
