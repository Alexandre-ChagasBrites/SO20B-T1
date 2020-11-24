#pragma once

#include <string>
#include <queue>
#include <vector>
#include "cpu.h"

class Temporizador
{
public:
    class Interrupcao
    {
    public:
        Interrupcao(bool periodica, unsigned int periodo, unsigned int data, std::string codigo);

        friend class Temporizador;

    private:
        unsigned int periodo;
        unsigned int data;
        bool periodica;
        std::string codigo;
    };

private:
    class Comparador
    {
    public:
        bool operator()(const Interrupcao &a, const Interrupcao &b) const
        {
            return a.data >= b.data;
        }
    };

public:
    Temporizador();

    void PassarTempo();
    unsigned int ObterTempo();

    std::string ObterInterrupcao();
    void PedirInterrupcao(bool periodica, unsigned int periodo, std::string codigo);

private:
    unsigned int tempo;
    std::priority_queue<Interrupcao, std::vector<Interrupcao>, Comparador> interrupcoes;
};
