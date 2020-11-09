#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "cpu.h"

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

    CPU cpu;

    CPU::Estado e;
    cpu.AlteraEstado(e);

    cpu.AlteraPrograma(programa);
    cpu.AlteraDados(dados);

    while (cpu.ObterInterrupcao() == CPU::Interrupcao::Normal)
        cpu.Executa();

    std::cout << cpu;

    //for (unsigned int i = 0; i < dados.size(); i++)
    //    std::cout << std::hex << i << " = " << std::dec << dados[i] << std::endl;

    return 0;
}
