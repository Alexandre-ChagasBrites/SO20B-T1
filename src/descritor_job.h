#pragma once

#include <string>
#include <vector>

class DescritorJob
{
public:
    class Dispositivo
    {
    public:
        Dispositivo();
        Dispositivo(unsigned int n, unsigned int tempo);
    public:
        unsigned int n;
        unsigned int tempo;
    };

public:
    DescritorJob(const char *arquivo);

public:
    std::vector<std::string> programa;
    unsigned int memoriaNecessaria;
    Dispositivo entrada;
    Dispositivo saida;
};
