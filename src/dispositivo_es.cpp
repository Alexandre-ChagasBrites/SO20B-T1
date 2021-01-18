#include "dispositivo_es.h"

#include <fstream>
#include <cstring>

DispositivoES::DispositivoES() : dados(0), indice(0) {}

DispositivoES::DispositivoES(std::string arquivo) : DispositivoES::DispositivoES()
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

        int a;
        int found = sscanf(c_str, "%i", &a);

        if (found == 1)
            dados.push_back(a);
    }
}

int DispositivoES::Ler()
{
    if (indice < dados.size())
        return dados[indice++];
    return 0;
}

void DispositivoES::Gravar(int a)
{
    dados.push_back(a);
}
