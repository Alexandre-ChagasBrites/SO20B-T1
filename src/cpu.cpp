#include "cpu.h"

#include <sstream>

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

CPU::CPU(MMU &mmu) : estado(), programa(nullptr), mmu(mmu)
{

}

void CPU::AlteraPrograma(std::vector<std::string> *programa)
{
    this->programa = programa;
}

CPU::Interrupcao CPU::ObterInterrupcao()
{
    return estado.interrupcao;
}

void CPU::RetornaInterrupcao()
{
    if (estado.interrupcao == Interrupcao::Normal)
        return;
    if (estado.interrupcao != Interrupcao::ViolacaoDeMemoria)
        estado.pc++;
    estado.interrupcao = Interrupcao::Normal;
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

void CPU::SalvaEstado(Estado &e)
{
    e = estado;
}

void CPU::AlteraEstado(Estado &e)
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

    std::stringstream instrucao(Instrucao());
    if (estado.interrupcao == Interrupcao::ViolacaoDeMemoria)
        return;

    bool incrementar = true;
    std::string comando;
    instrucao >> comando;

    if (comando == "CARGI")
    {
        //CARGI	n	coloca o valor n no acumulador (A=n)
        int n;
        instrucao >> n;
        estado.acumulador = n;
        //printf("A=%i\n", n);
    }
    else if (comando == "CARGM")
    {
        //CARGM	n	coloca no acumulador o valor na posi��o n da mem�ria de dados (A=M[n])
        int n;
        instrucao >> n;
        estado.acumulador = MemoriaLer(n);
        //printf("A=M[%i]\n", n);
    }
    else if (comando == "CARGX")
    {
        //CARGX	n	coloca no acumulador o valor na posi��o que est� na posi��o n da mem�ria de dados (A=M[M[n]])
        int n;
        instrucao >> n;

        int i = MemoriaLer(n);
        if (estado.interrupcao != Interrupcao::ViolacaoDeMemoria)
            estado.acumulador = MemoriaLer(i);
        //printf("A=M[M[%i]]\n", n);
    }
    else if (comando == "ARMM")
    {
        //ARMM	n	coloca o valor do acumulador na posi��o n da mem�ria de dados (M[n]=A)
        int n;
        instrucao >> n;
        MemoriaEscrever(n, estado.acumulador);
        //printf("M[%i]=A\n", n);
    }
    else if (comando == "ARMX")
    {
        //ARMX	n	coloca o valor do acumulador posi��o que est� na posi��o n da mem�ria de dados (M[M[n]]=A)
        int n;
        instrucao >> n;

        int i = MemoriaLer(n);
        if (estado.interrupcao != Interrupcao::ViolacaoDeMemoria)
            MemoriaEscrever(i, estado.acumulador);
        //printf("M[M[%i]]=A\n", n);
    }
    else if (comando == "SOMA")
    {
        //SOMA	n	soma ao acumulador o valor no endere�o n da mem�ria de dados (A=A+M[n])
        int n;
        instrucao >> n;
        estado.acumulador += MemoriaLer(n);
        //printf("A=A+M[%i]\n", n);
    }
    else if (comando == "NEG")
    {
        //NEG		inverte o sinal do acumulador (A=-A)
        estado.acumulador = -estado.acumulador;
        //printf("A=-A\n");
    }
    else if (comando == "DESVZ")
    {
        //DESVZ	n	se A vale 0, coloca o valor n no PC
        int n;
        instrucao >> n;
        if (estado.acumulador == 0)
        {
            estado.pc = n;
            incrementar = false;
        }
        //printf("if (A == 0) PC=%i\n", n);
    }
    else
    {
        //outra		coloca a CPU em interrup��o � instru��o ilegal
        estado.interrupcao = Interrupcao::InstrucaoIlegal;
        incrementar = false;
    }
    //printf("\n");

    if (incrementar && estado.interrupcao != Interrupcao::ViolacaoDeMemoria)
        estado.pc++;
}

int CPU::MemoriaLer(unsigned int i)
{
    int valor = 0;
    MMU::Erro erro = mmu.Ler(i, valor);
    if (erro == MMU::Erro::AcessoInvalido || erro == MMU::Erro::PaginaInvalida)
        estado.interrupcao = Interrupcao::ViolacaoDeMemoria;
    return valor;
}

void CPU::MemoriaEscrever(unsigned int i, int valor)
{
    MMU::Erro erro = mmu.Alterar(i, valor);
    if (erro == MMU::Erro::AcessoInvalido || erro == MMU::Erro::PaginaInvalida)
        estado.interrupcao = Interrupcao::ViolacaoDeMemoria;
}

std::ostream &operator<<(std::ostream &os, CPU &cpu)
{
    os << "CPU:\n";
    os << " Instrucao: " << cpu.Instrucao() << std::endl;
    os << cpu.estado;
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
