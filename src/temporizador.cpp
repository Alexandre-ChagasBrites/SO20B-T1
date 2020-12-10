#include "temporizador.h"

Temporizador::Interrupcao::Interrupcao(bool periodica, unsigned int periodo, unsigned int data, std::string codigo) :
    periodo(periodo), data(data), periodica(periodica), codigo(codigo) {}

Temporizador::Temporizador() : tempo(0), interrupcoes() {}

void Temporizador::PassarTempo()
{
    tempo++;
}

unsigned int Temporizador::ObterTempo()
{
    return tempo;
}

std::string Temporizador::ObterInterrupcao()
{
    if (interrupcoes.empty())
        return "";

    Interrupcao interrupcao = interrupcoes.top();
    if (tempo < interrupcao.data)
        return "";

    interrupcoes.pop();
    if (interrupcao.periodica)
        interrupcoes.push(Interrupcao(interrupcao.periodica, interrupcao.periodo, interrupcao.data + interrupcao.periodo, interrupcao.codigo));

    return interrupcao.codigo;
}

void Temporizador::PedirInterrupcao(bool periodica, unsigned int periodo, std::string codigo)
{
    interrupcoes.push(Interrupcao(periodica, periodo, tempo + periodo, codigo));
}

std::ostream &operator<<(std::ostream &os, const Temporizador &temporizador)
{
    os << "Temporizador = " << temporizador.tempo << std::endl;
    return os;
}
