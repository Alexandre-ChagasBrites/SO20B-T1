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
        Dispositivo(std::string nome, unsigned int tempoLeitura, unsigned int tempoGravacao);
    public:
        std::string nome;
        unsigned int tempoLeitura;
        unsigned int tempoGravacao;
    };

public:
    DescritorJob(std::string arquivo, unsigned int dataLancamento);
    friend class SO;
    friend class DescritorProcesso;

private:
    std::string arquivo;
    std::vector<std::string> programa;
    unsigned int memoriaNecessaria;
    std::vector<Dispositivo> dispositivos;
    unsigned int dataLancamento;
};
