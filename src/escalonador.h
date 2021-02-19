#pragma once

#include <vector>
#include "descritor_processo.h"

class Escalonador
{
public:
    int RetornaProcesso(int processoAtual, std::vector<DescritorProcesso*> *processos);
};
