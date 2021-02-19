#include "mmu.h"

MMU::MMU(Memoria &memoriaFisica) : memoriaFisica(memoriaFisica), tabela(nullptr), erro(Erro::Nenhum), paginaLida(0)
{
    
}

MMU::Erro MMU::Ler(unsigned int i, int &valor)
{
    unsigned int paginaIndice = i / memoriaFisica.TamanhoQuadro();
    paginaLida = paginaIndice;
    if (paginaIndice >= tabela->size())
        return erro = Erro::AcessoInvalido;

    DescritorPagina &pagina = (*tabela)[paginaIndice];
    if (!pagina.valida)
        return erro = Erro::PaginaInvalida;

    pagina.acessada = true;
    valor = memoriaFisica.LerInteiro(pagina.quadro * memoriaFisica.TamanhoQuadro() + i % memoriaFisica.TamanhoQuadro());
    return Erro::Nenhum;
}

MMU::Erro MMU::Alterar(unsigned int i, int valor)
{
    unsigned int paginaIndice = i / memoriaFisica.TamanhoQuadro();
    paginaLida = paginaIndice;
    if (paginaIndice >= tabela->size())
        return erro = Erro::AcessoInvalido;

    DescritorPagina &pagina = (*tabela)[paginaIndice];
    if (!pagina.valida || !pagina.alteravel)
        return erro = Erro::PaginaInvalida;

    pagina.acessada = true;
    pagina.alterada = true;
    memoriaFisica.AlterarInteiro(pagina.quadro * memoriaFisica.TamanhoQuadro() + i % memoriaFisica.TamanhoQuadro(), valor);
    return Erro::Nenhum;
}

void MMU::UsarTabela(std::vector<DescritorPagina> *tabela)
{
    this->tabela = tabela;
}

MMU::Erro MMU::ObterErro()
{
    return erro;
}

unsigned int MMU::ObterPagina()
{
    return paginaLida;
}
