#include "descritor_job.h"

#include <fstream>
#include <sstream>

DescritorJob::Dispositivo::Dispositivo() : nome(""), tempoLeitura(0), tempoGravacao(0) {}
DescritorJob::Dispositivo::Dispositivo(std::string nome, unsigned int tempoLeitura, unsigned int tempoGravacao) :
    nome(nome), tempoLeitura(tempoLeitura), tempoGravacao(tempoGravacao) {}

DescritorJob::DescritorJob(std::string arquivo, unsigned int dataLancamento) :
    arquivo(arquivo), programa(0), memoriaNecessaria(0), dispositivos(0), dataLancamento(dataLancamento)
{
    std::vector<std::string> linhas;

    std::ifstream file(arquivo);
    {
        std::string linha;
        while (std::getline(file, linha))
            linhas.push_back(linha);
    }
    file.close();

    unsigned int tipo = 0;
    for (unsigned int i = 0; i < linhas.size(); i++)
    {
        std::stringstream linha(linhas[i]);
        if (tipo == 0)
        {
            std::string comando;
            linha >> comando;

            if (comando == "#MEMORIA")
            {
                unsigned int quantidade;
                linha >> quantidade;
                memoriaNecessaria = quantidade;
            }
            else if (comando == "#PROGRAMA")
                tipo = 1;
            else if (comando == "#DISPOSITIVOS")
                tipo = 2;
        }
        else
        {
            if (linha.str() == "#FIM")
            {
                tipo = 0;
                continue;
            }

            if (tipo == 1)
            {
                if (linha.str().find(';') != std::string::npos)
                    continue;
                programa.push_back(linhas[i]);
            }
            else if (tipo == 2 || tipo == 3)
            {
                std::string nome;
                unsigned int tempoLeitura;
                unsigned int tempoGravacao;

                linha >> nome;
                linha >> tempoLeitura;
                linha >> tempoGravacao;

                dispositivos.push_back(Dispositivo(nome, tempoLeitura, tempoGravacao));
            }
        }
    }
}
