#include "cpu.h"

#include <cstring>

CPU::Estado::Estado() : pc(0), acumulador(0), interrupcao(CPU::Interrupcao::Normal) {}

void CPU::Estado::Inicializa()
{
    pc = 0;
    acumulador = 0;
    interrupcao = CPU::Interrupcao::Normal;
}

void CPU::Estado::AlteraAcumulador(int acumulador)
{
    this->acumulador = acumulador;
}

int CPU::Estado::ObterAcumulador()
{
    return acumulador;
}

void CPU::AlteraPrograma(std::vector<std::string> *programa)
{
    this->programa = programa;
}

void CPU::AlteraDados(std::vector<int> *dados)
{
    this->dados = dados;
}

CPU::Interrupcao CPU::ObterInterrupcao()
{
    return estado.interrupcao;
}

void CPU::RetornaInterrupcao()
{
    if (estado.interrupcao == Interrupcao::Normal)
        return;
    estado.interrupcao = Interrupcao::Normal;
    estado.pc++;
}

std::string CPU::Instrucao()
{
    if (estado.pc >= programa->size())
    {
        estado.interrupcao = Interrupcao::ViolacaoDeMemoria;
        return "";
    }
    return (*programa)[estado.pc];
}

void CPU::SalvaEstado(Estado *e)
{
    *e = estado;
}

void CPU::AlteraEstado(Estado e)
{
    estado = e;
}

void CPU::Dormir()
{
    estado.interrupcao = Interrupcao::Dormindo;
}

void CPU::Executa()
{
    if (estado.interrupcao != Interrupcao::Normal)
        return;

    std::string instrucao = Instrucao();
    if (estado.interrupcao == Interrupcao::ViolacaoDeMemoria)
        return;

    bool incrementar = true;

    const char *c_instrucao = instrucao.c_str();
    if (std::strstr(c_instrucao, "CARGI"))
    {
        //CARGI	n	coloca o valor n no acumulador (A=n)
        int n;
        std::sscanf(c_instrucao, "%*s %i", &n);
        estado.acumulador = n;
        //printf("A=%i\n", n);
    }
    else if (std::strstr(c_instrucao, "CARGM"))
    {
        //CARGM	n	coloca no acumulador o valor na posição n da memória de dados (A=M[n])
        int n;
        std::sscanf(c_instrucao, "%*s %i", &n);
        estado.acumulador = MemoriaLer(n);
        //printf("A=M[%i]\n", n);
    }
    else if (std::strstr(c_instrucao, "CARGX"))
    {
        //CARGX	n	coloca no acumulador o valor na posição que está na posição n da memória de dados (A=M[M[n]])
        int n;
        std::sscanf(c_instrucao, "%*s %i", &n);
        estado.acumulador = MemoriaLer(MemoriaLer(n));
        //printf("A=M[M[%i]]\n", n);
    }
    else if (std::strstr(c_instrucao, "ARMM"))
    {
        //ARMM	n	coloca o valor do acumulador na posição n da memória de dados (M[n]=A)
        int n;
        std::sscanf(c_instrucao, "%*s %i", &n);
        MemoriaEscrever(n, estado.acumulador);
        //printf("M[%i]=A\n", n);
    }
    else if (std::strstr(c_instrucao, "ARMX"))
    {
        //ARMX	n	coloca o valor do acumulador posição que está na posição n da memória de dados (M[M[n]]=A)
        int n;
        std::sscanf(c_instrucao, "%*s %i", &n);
        MemoriaEscrever(MemoriaLer(n), estado.acumulador);
        //printf("M[M[%i]]=A\n", n);
    }
    else if (std::strstr(c_instrucao, "SOMA"))
    {
        //SOMA	n	soma ao acumulador o valor no endereço n da memória de dados (A=A+M[n])
        int n;
        std::sscanf(c_instrucao, "%*s %i", &n);
        estado.acumulador += MemoriaLer(n);
        //printf("A=A+M[%i]\n", n);
    }
    else if (std::strstr(c_instrucao, "NEG"))
    {
        //NEG		inverte o sinal do acumulador (A=-A)
        estado.acumulador = -estado.acumulador;
        //printf("A=-A\n");
    }
    else if (std::strstr(c_instrucao, "DESVZ"))
    {
        //DESVZ	n	se A vale 0, coloca o valor n no PC
        int n;
        std::sscanf(c_instrucao, "%*s %i", &n);
        if (estado.acumulador == 0)
        {
            estado.pc = n;
            incrementar = false;
        }
        //printf("if (A == 0) PC=%i\n", n);
    }
    else
    {
        //outra		coloca a CPU em interrupção – instrução ilegal
        estado.interrupcao = Interrupcao::InstrucaoIlegal;
        incrementar = false;
    }
    //printf("\n");

    if (incrementar && estado.interrupcao != Interrupcao::ViolacaoDeMemoria)
        estado.pc++;
}

int CPU::MemoriaLer(unsigned int i)
{
    if (i >= dados->size())
    {
        estado.interrupcao = Interrupcao::ViolacaoDeMemoria;
        return 0;
    }
    return (*dados)[i];
}

void CPU::MemoriaEscrever(unsigned int i, int valor)
{
    if (i >= dados->size())
    {
        estado.interrupcao = Interrupcao::ViolacaoDeMemoria;
        return;
    }
    (*dados)[i] = valor;
}

std::ostream &operator<<(std::ostream &os, CPU &cpu)
{
    os << "CPU:\n";
    os << " Instrucao: " << cpu.Instrucao() << std::endl;
    os << cpu.estado;
    os << " Memoria de dados:\n";
    for (unsigned int i = 0; i < cpu.dados->size(); i++)
        os << "  " << std::hex << i << " = " << std::dec << (*cpu.dados)[i] << std::endl;
    return os;
}

std::ostream &operator<<(std::ostream &os, const CPU::Estado &estado)
{
    os << " PC = " << estado.pc << std::endl;
    os << " Acumulador = " << estado.acumulador << std::endl;
    os << " Estado = ";
    switch (estado.interrupcao)
    {
        case CPU::Interrupcao::Normal:
            std::cout << "Normal\n";
            break;
        case CPU::Interrupcao::InstrucaoIlegal:
            std::cout << "Instrucao ilegal\n";
            break;
        case CPU::Interrupcao::ViolacaoDeMemoria:
            std::cout << "Violacao de memoria\n";
            break;
        case CPU::Interrupcao::Dormindo:
            std::cout << "Dormindo\n";
            break;
    }
    return os;
}
