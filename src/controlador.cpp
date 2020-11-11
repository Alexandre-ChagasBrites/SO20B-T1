#include "controlador.h"

#include "so.h"
#include "cpu.h"

void Controlador::Executa(SO *so, CPU *cpu)
{
    while (true)
    {
        cpu->Executa();

        if (cpu->ObterInterrupcao() != CPU::Interrupcao::Normal)
        {
            bool continuar = true;
            switch (cpu->ObterInterrupcao())
            {
            case CPU::Interrupcao::InstrucaoIlegal:
                continuar = so->InstrucaoIlegal();
                break;
            case CPU::Interrupcao::ViolacaoDeMemoria:
                continuar = so->ViolacaoDeMemoria();
                break;
            default:
                break;
            }

            if (!continuar)
                break;
        }

        temporizador.PassarTempo();
        while (temporizador.ObterInterrupcao() != CPU::Interrupcao::Normal)
        {

        }
    }
}
