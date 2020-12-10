#include "so.h"

#include <iostream>
#include <cstring>

void SO::Executa()
{
    std::vector<int> dados(8);

    for (unsigned int i = 0; i < jobs.size(); i++)
    {
        DescritorJob &job = jobs[i];

        estado.Inicializa();
        cpu.AlteraEstado(estado);

        cpu.AlteraPrograma(&job.programa);
        cpu.AlteraDados(&dados);

        controlador.Executa(this, &cpu, &temporizador);

        std::cout << cpu << std::endl;
    }

    std::cout << temporizador;
}

void SO::CarregarDispositivo(const char *arquivo)
{
    dispositivos.push_back(DispositivoES(arquivo));
}

void SO::CarregarJob(const char *arquivo)
{
    jobs.push_back(DescritorJob(arquivo));
}

bool SO::InstrucaoIlegal()
{
    std::string instrucao = cpu.Instrucao();

    const char *c_instrucao = instrucao.c_str();
    if (std::strstr(c_instrucao, "PARA"))
    {
        //PARA	n	pede ao SO para terminar a execuчуo do programa (como exit)
        return false;
    }
    else if (std::strstr(c_instrucao, "LE"))
    {
        //LE	n	pede ao SO para fazer a leitura de um dado (inteiro) do dispositivo de E/S n;
        //          o dado sera colocado no acumulador
        int n;
        std::sscanf(c_instrucao, "%*s %i", &n);

        cpu.SalvaEstado(&estado);
        estado.AlteraAcumulador(dispositivos[n].Ler());

        cpu.Dormir();
        temporizador.PedirInterrupcao(false, 10, "DORMINDO");

        return true;
    }
    else if (std::strstr(c_instrucao, "GRAVA"))
    {
        //GRAVA	n	pede ao SO gravar o valor do acumulador no dispositivo de E/S n
        int n;
        std::sscanf(c_instrucao, "%*s %i", &n);

        cpu.SalvaEstado(&estado);
        dispositivos[n].Gravar(estado.ObterAcumulador());

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
