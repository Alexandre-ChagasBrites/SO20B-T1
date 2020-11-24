#include "controlador.h"

#include "so.h"
#include "cpu.h"

void Controlador::Executa(SO *so, CPU *cpu)
{
    while (true)
    {
        if (cpu->ObterInterrupcao() != CPU::Interrupcao::Dormindo)
        {
            cpu->Executa();

            bool continuar = true;
            switch (cpu->ObterInterrupcao())
            {
            case CPU::Interrupcao::InstrucaoIlegal:
                continuar = so->InstrucaoIlegal(temporizador);
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
        for (std::string interrupcao = temporizador.ObterInterrupcao(); interrupcao != ""; interrupcao = temporizador.ObterInterrupcao())
        {
            if (interrupcao == "DORMINDO")
            {
                cpu->AlteraEstado(so->estado);
                cpu->RetornaInterrupcao();
            }
        }
    }
}
