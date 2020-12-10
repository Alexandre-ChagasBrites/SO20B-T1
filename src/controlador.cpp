#include "controlador.h"

#include "so.h"
#include "cpu.h"

void Controlador::Executa(SO *so, CPU *cpu, Temporizador *temporizador)
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

        for (std::string interrupcao = temporizador->ObterInterrupcao(); interrupcao != ""; interrupcao = temporizador->ObterInterrupcao())
        {
            if (interrupcao == "DORMINDO")
            {
                cpu->AlteraEstado(so->estado);
                cpu->RetornaInterrupcao();
            }
        }
        temporizador->PassarTempo();
    }
}
