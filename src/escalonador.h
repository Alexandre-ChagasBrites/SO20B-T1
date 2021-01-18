#pragma once

#include <vector>
#include "descritor_processo.h"

class Escalonador
{
public:
    DescritorProcesso *RetornaProcesso(DescritorProcesso *processoExecucao, std::vector<DescritorProcesso*> *processos);
};
