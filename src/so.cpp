#include "so.h"

#include <iostream>
#include <cstring>

void SO::Inicializa(std::vector<std::string> *programa, std::vector<int> *dados)
{
    CPU::Estado e;
    cpu.AlteraEstado(e);

    cpu.AlteraPrograma(programa);
    cpu.AlteraDados(dados);

    controlador.Executa(this, &cpu);
}

bool SO::InstrucaoIlegal(Temporizador &temporizador)
{
    std::string instrucao = cpu.Instrucao();

    const char *c_instrucao = instrucao.c_str();
    if (std::strstr(c_instrucao, "PARA"))
    {
        //PARA	n	pede ao SO para terminar a execução do programa (como exit)
        int n;
        std::sscanf(c_instrucao, "%*s %i", &n);

        return false;
    }
    else if (std::strstr(c_instrucao, "LE"))
    {
        //LE	n	pede ao SO para fazer a leitura de um dado (inteiro) do dispositivo de E/S n; o dado
        //          o dado será colocado no acumulador
        int n;
        //std::sscanf(c_instrucao, "%*s %i", &n);
        //estado.acumulador = n;

        cpu.SalvaEstado(&estado);
        cpu.Dormir();
        temporizador.PedirInterrupcao(false, 10, "DORMINDO");

        return true;
    }
    else if (std::strstr(c_instrucao, "GRAVA"))
    {
        //GRAVA	n	pede ao SO gravar o valor do acumulador no dispositivo de E/S n
        int n;
        //std::sscanf(c_instrucao, "%*s %i", &n);

        cpu.SalvaEstado(&estado);
        cpu.Dormir();
        temporizador.PedirInterrupcao(false, 10, "DORMINDO");

        return true;
    }

    return true;
}

bool SO::ViolacaoDeMemoria()
{
    std::cout << "Violacao de memoria" << std::endl;
    return false;
}
