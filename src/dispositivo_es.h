#pragma once

#include <vector>

class DispositivoES
{
public:
    DispositivoES();
    DispositivoES(const char *arquivo);

    int Ler();
    void Gravar(int a);

private:
    std::vector<int> dados;
    unsigned int indice;
};
