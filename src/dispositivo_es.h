#pragma once

#include <vector>
#include <string>

class DispositivoES
{
public:
    DispositivoES();
    DispositivoES(std::string arquivo);

    int Ler();
    void Gravar(int a);

private:
    std::vector<int> dados;
    unsigned int indice;
};
