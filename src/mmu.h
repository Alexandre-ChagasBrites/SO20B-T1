#pragma once

#include <vector>
#include "memoria.h"

class MMU
{
public:
    enum class Erro
    {
        Nenhum, PaginaInvalida, AcessoInvalido
    };

    struct DescritorPagina
    {
        bool valida = false;
        unsigned int quadro = 0;
        bool alteravel = true;
        bool acessada = false;
        bool alterada = false;
    };

public:
    MMU(Memoria &memoria);

    Erro Ler(unsigned int i, int &valor);
    Erro Alterar(unsigned int i, int valor);

    void UsarTabela(std::vector<DescritorPagina> *tabela);

    Erro ObterErro();
    unsigned int ObterPagina();

private:
    Memoria &memoriaFisica;
    std::vector<DescritorPagina> *tabela;
    Erro erro;
    unsigned int paginaLida;
};
