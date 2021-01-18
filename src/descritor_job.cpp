#include "descritor_job.h"

#include <fstream>
#include <cstring>

DescritorJob::Dispositivo::Dispositivo() : nome(""), tempoLeitura(0), tempoGravacao(0) {}
DescritorJob::Dispositivo::Dispositivo(std::string nome, unsigned int tempoLeitura, unsigned int tempoGravacao) :
    nome(nome), tempoLeitura(tempoLeitura), tempoGravacao(tempoGravacao) {}

DescritorJob::DescritorJob(std::string arquivo, unsigned int dataLancamento) :
    arquivo(arquivo), programa(0), memoriaNecessaria(0), dispositivos(0), dataLancamento(dataLancamento)
{
    std::vector<std::string> strings;
    std::ifstream file(arquivo);
    std::string str;
    while (std::getline(file, str))
        strings.push_back(str);
    file.close();

    unsigned int tipo = 0;
    for (unsigned int i = 0; i < strings.size(); i++)
    {
        const char *c_str = strings[i].c_str();

        if (tipo == 0)
        {
            if (std::strstr(c_str, "#MEMORIA"))
            {
                unsigned int quantidade;
                int found = sscanf(c_str, "%*s %u", &quantidade);

                if (found == 1)
                    memoriaNecessaria = quantidade;
            }
            else if (std::strstr(c_str, "#PROGRAMA"))
                tipo = 1;
            else if (std::strstr(c_str, "#DISPOSITIVOS"))
                tipo = 2;
        }
        else
        {
            if (std::strstr(c_str, "#FIM"))
            {
                tipo = 0;
                continue;
            }

            if (tipo == 1)
            {
                unsigned char c;
                int found = sscanf(c_str, "%c", &c);
                if (found && c == ';')
                    continue;

                programa.push_back(strings[i]);
            }
            else if (tipo == 2 || tipo == 3)
            {
                char nome[64];
                unsigned int tempoLeitura;
                unsigned int tempoGravacao;
                int found = sscanf(c_str, "%s %u %u", nome, &tempoLeitura, &tempoGravacao);

                if (found == 3)
                    dispositivos.push_back(Dispositivo(nome, tempoLeitura, tempoGravacao));
            }
        }
    }
}
