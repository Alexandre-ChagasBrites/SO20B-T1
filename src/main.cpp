#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "so.h"
#include "temporizador.h"

std::vector<std::string> CarregaPrograma(const char *path)
{
    std::vector<std::string> strings;
    std::ifstream file(path);

    std::string str;
    while (std::getline(file, str))
        strings.push_back(str);

    file.close();
    return strings;
}

void TesteTemporizador()
{
    Temporizador timer;
    timer.PedirInterrupcao(true, 3, "pa");
    timer.PedirInterrupcao(true, 5, "pum");
    timer.PedirInterrupcao(false, 29, "fim");

    bool termina = false;
    while (!termina)
    {
        timer.PassarTempo();

        std::string tipo = timer.ObterInterrupcao();
        if (tipo == "")
            std::cout << timer.ObterTempo() << std::endl;
        else do
        {
            std::cout << tipo << std::endl;
            if (tipo == "fim")
                termina = true;
            tipo = timer.ObterInterrupcao();
        }
        while(tipo != "");
    }
}

int main(int argc, char *argv[])
{
    std::vector<std::string> programa = CarregaPrograma("programa.txt");
    std::vector<int> dados(4);

    SO so;
    so.Inicializa(&programa, &dados);

    std::cout << so.cpu;

    return 0;
}
