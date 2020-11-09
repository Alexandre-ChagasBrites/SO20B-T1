#pragma once

#include <iostream>
#include <string>
#include <array>
#include <vector>

class CPU
{
public:
    enum class Interrupcao
    {
        Normal, InstrucaoIlegal, ViolacaoDeMemoria
    };

    struct Estado
    {
        Estado() : pc(0), acumulador(0), interrupcao(Interrupcao::Normal) {}

        unsigned int pc;
        int acumulador;
        Interrupcao interrupcao;

        friend std::ostream &operator<<(std::ostream &os, const Estado &estado);
    };

public:
    //CPU() {};
    //~CPU() {};

    void AlteraPrograma(std::vector<std::string> programa);
    void AlteraDados(std::vector<int> dados);

    Interrupcao ObterInterrupcao();
    void RetornaInterrupcao();
    std::string Instrucao();

    void SalvaEstado(Estado &e);
    void AlteraEstado(Estado &e);
    void Executa();

    friend std::ostream &operator<<(std::ostream &os, CPU &cpu);

private:
    int MemoriaLer(unsigned int i);
    void MemoriaEscrever(unsigned int i, int valor);

private:
    Estado estado;
    std::vector<std::string> programa;
    std::vector<int> dados;
};
