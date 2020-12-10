#include "descritor_job.h"

#include <fstream>
#include <cstring>

DescritorJob::Dispositivo::Dispositivo() : n(0), tempo(0) {}
DescritorJob::Dispositivo::Dispositivo(unsigned int n, unsigned int tempo) : n(n), tempo(tempo) {}

DescritorJob::DescritorJob(const char *arquivo) : programa(0), memoriaNecessaria(0), entrada(), saida()
{
    std::vector<std::string> strings;
    std::ifstream file(arquivo);
    std::string str;
    while (std::getline(file, str))
        strings.push_back(str);
    file.close();

    for (std::string str : strings)
    {
        const char *c_str = str.c_str();
        if (std::strstr(c_str, "#PROGRAMA"))
        {
            unsigned int startPrograma, countPrograma;
            int found = sscanf(c_str, "%*s %u %u", &startPrograma, &countPrograma);

            if (found == 2)
            {
                for (unsigned int i = 0; i < countPrograma; i++)
                    programa.push_back(strings[startPrograma + i]);
            }
        }
        else if (std::strstr(c_str, "#MEMORIA"))
        {
            unsigned int quantidade;
            int found = sscanf(c_str, "%*s %u", &quantidade);

            if (found == 1)
                memoriaNecessaria = quantidade;
        }
        else if (std::strstr(c_str, "#ENTRADA") || std::strstr(c_str, "#SAIDA"))
        {
            unsigned int n;
            unsigned int tempo;
            int found = sscanf(c_str, "%*s %u %u", &n, &tempo);

            if (found == 2)
            {
                if (std::strstr(c_str, "#ENTRADA"))
                    entrada = Dispositivo(n, tempo);
                else
                    saida = Dispositivo(n, tempo);
            }
        }
    }
}
