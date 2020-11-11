#include "temporizador.h"

Temporizador::Interrupcao::Interrupcao(Temporizador::Interrupcao::Tipo tipo, unsigned int periodo, unsigned int data, CPU::Interrupcao codigo) :
    tipo(tipo), periodo(periodo), data(data), codigo(codigo) {}

void Temporizador::PassarTempo()
{
    tempo++;
}

unsigned int Temporizador::ObterTempo()
{
    return tempo;
}

CPU::Interrupcao Temporizador::ObterInterrupcao()
{
    if (interrupcoes.empty())
        return CPU::Interrupcao::Normal;

    Interrupcao interrupcao = interrupcoes.front();
    if (tempo < interrupcao.data)
        return CPU::Interrupcao::Normal;

    interrupcoes.pop();
    if (interrupcao.tipo == Interrupcao::Tipo::Periodica)
        interrupcoes.push(Interrupcao(interrupcao.tipo, interrupcao.periodo, interrupcao.data + interrupcao.periodo, interrupcao.codigo));

    return interrupcao.codigo;
}

void Temporizador::PedirInterrupcao(Interrupcao::Tipo tipo, unsigned int periodo, CPU::Interrupcao codigo)
{
    interrupcoes.push(Interrupcao(tipo, periodo, tempo + periodo, codigo));
}
