#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "so.h"

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

int main(int argc, char *argv[])
{
    std::vector<std::string> programa = CarregaPrograma("programa.txt");
    std::vector<int> dados(4);

    SO so;
    so.Inicializa(&programa, &dados);

    std::cout << so.cpu;

    return 0;
}
