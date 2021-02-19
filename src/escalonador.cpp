#include "escalonador.h"

int Escalonador::RetornaProcesso(int processoAtual, std::vector<DescritorProcesso*> *processos)
{
    if (processoAtual != -1)
    {
        if ((*processos)[processoAtual]->estado != DescritorProcesso::Estado::Bloqueado &&
            (*processos)[processoAtual]->estado != DescritorProcesso::Estado::BloqueadoFaltaPagina &&
            (*processos)[processoAtual]->estado != DescritorProcesso::Estado::Terminado &&
            (*processos)[processoAtual]->quantum >= 1)
            return processoAtual;
    }

    int proximoProcesso = -1;
    for (unsigned int i = 0; i < processos->size(); i++)
    {
        DescritorProcesso *processo = (*processos)[i];
        if (processo->estado == DescritorProcesso::Estado::Pronto)
        {
            if (proximoProcesso == -1 || processo->prioridade < (*processos)[proximoProcesso]->prioridade)
                proximoProcesso = i;
        }
    }

    return proximoProcesso;
}
