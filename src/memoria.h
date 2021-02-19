#pragma once

#include <vector>

class Memoria
{
public:
    struct Quadro
    {
        std::vector<int> dados;
    };

public:
    Memoria(unsigned int tamanhoPagina, unsigned int tamanhoTotal);
    Memoria(unsigned int memoriaNecessaria, Memoria &memoriaReferencia);

    int LerInteiro(unsigned int i);
    void AlterarInteiro(unsigned int i, int valor);

    Quadro &LerQuadro(unsigned int i);
    void AlterarQuadro(unsigned int i, Quadro &valor);

    const unsigned int TamanhoQuadro();
    const unsigned int TamanhoTotal();

private:
    unsigned int tamanhoQuadro;
    std::vector<Quadro> quadros;
};
