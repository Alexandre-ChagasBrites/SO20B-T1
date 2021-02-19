#include "memoria.h"

Memoria::Memoria(unsigned int tamanhoPagina, unsigned int tamanhoTotal) :
    tamanhoQuadro(tamanhoPagina), quadros(tamanhoTotal)
{
    for (unsigned int i = 0; i < tamanhoTotal; i++)
        quadros[i].dados.resize(tamanhoPagina);
}

Memoria::Memoria(unsigned int memoriaNecessaria, Memoria &memoriaReferencia)
{
    unsigned int paginas = memoriaNecessaria / memoriaReferencia.TamanhoQuadro();
    if ((memoriaNecessaria % memoriaReferencia.TamanhoQuadro()) != 0)
        paginas++;

    tamanhoQuadro = memoriaReferencia.tamanhoQuadro;
    quadros.resize(paginas);

    for (unsigned int i = 0; i < paginas; i++)
        quadros[i].dados.resize(tamanhoQuadro);
}

int Memoria::LerInteiro(unsigned int i)
{
    return quadros[i / tamanhoQuadro].dados[i % tamanhoQuadro];
}

void Memoria::AlterarInteiro(unsigned int i, int valor)
{
    quadros[i / tamanhoQuadro].dados[i % tamanhoQuadro] = valor;
}

Memoria::Quadro &Memoria::LerQuadro(unsigned int i)
{
    return quadros[i];
}

void Memoria::AlterarQuadro(unsigned int i, Quadro &valor)
{
    quadros[i] = valor;
}

const unsigned int Memoria::TamanhoQuadro()
{
    return tamanhoQuadro;
}

const unsigned int Memoria::TamanhoTotal()
{
    return quadros.size();
}
