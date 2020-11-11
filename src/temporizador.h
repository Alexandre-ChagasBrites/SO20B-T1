#pragma once

#include <queue>
#include "cpu.h"

class Temporizador
{
public:
    class Interrupcao
    {
    public:
        enum class Tipo
        {
            Periodica, Aperiodica
        };

    public:
        Interrupcao(Tipo tipo, unsigned int periodo, unsigned int data, CPU::Interrupcao codigo);

        friend class Temporizador;

    private:
        Tipo tipo;
        unsigned int periodo;
        unsigned int data;
        CPU::Interrupcao codigo;
    };

public:
    void PassarTempo();
    unsigned int ObterTempo();

    CPU::Interrupcao ObterInterrupcao();
    void PedirInterrupcao(Interrupcao::Tipo tipo, unsigned int periodo, CPU::Interrupcao codigo);

private:
    unsigned int tempo;
    std::queue<Interrupcao> interrupcoes;
};
