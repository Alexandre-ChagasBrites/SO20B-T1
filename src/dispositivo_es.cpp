#include "dispositivo_es.h"

#include <fstream>
#include <sstream>

DispositivoES::DispositivoES() : dados(0), indice(0) {}

DispositivoES::DispositivoES(std::string arquivo) : DispositivoES::DispositivoES()
{
    std::vector<std::string> linhas;

    std::ifstream file(arquivo);
    {
        std::string linha;
        while (std::getline(file, linha))
            linhas.push_back(linha);
    }
    file.close();

    for (std::string linha : linhas)
    {
        int a = std::stoi(linha);
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
