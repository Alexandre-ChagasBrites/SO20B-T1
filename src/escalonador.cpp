#include "escalonador.h"

DescritorProcesso *Escalonador::RetornaProcesso(DescritorProcesso *processoExecucao, std::vector<DescritorProcesso*> *processos)
{
    if (processoExecucao != nullptr)
    {
        if (processoExecucao->estado != DescritorProcesso::Estado::Bloqueado &&
            processoExecucao->estado != DescritorProcesso::Estado::Terminado &&
            processoExecucao->quantum >= 1)
            return processoExecucao;
    }

    DescritorProcesso *proximoProcesso = nullptr;
    for (unsigned int i = 0; i < processos->size(); i++)
    {
        DescritorProcesso *processo = (*processos)[i];
        if (processo->estado == DescritorProcesso::Estado::Pronto)
        {
            if (proximoProcesso == nullptr || processo->prioridade < proximoProcesso->prioridade)
                proximoProcesso = processo;
        }
    }

    return proximoProcesso;
}
