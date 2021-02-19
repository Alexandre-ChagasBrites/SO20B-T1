#pragma once

#include <vector>
#include <deque>
#include <string>
#include "memoria.h"
#include "mmu.h"
#include "cpu.h"
#include "controlador.h"
#include "temporizador.h"
#include "escalonador.h"
#include "descritor_job.h"
#include "descritor_processo.h"
#include "dispositivo_es.h"

class SO
{
public:
    enum class TrocaDePagina
    {
        FIFO, SegundaChance
    };

    enum class Prioridade
    {
        Fixa, Abaixo, Acima
    };

    struct DescritorQuadro
    {
        bool livre = true;
        unsigned int processo;
        unsigned int pagina;
        bool alvo = false;
    };

    struct Configuracao
    {
        unsigned int tamanhoPagina = 4;
        unsigned int tamanhoTotal = 8;
        TrocaDePagina trocaDePagina = TrocaDePagina::FIFO;
        unsigned int quantum = 10;
        Prioridade prioridade = Prioridade::Abaixo;
    };

public:
    SO(unsigned int tamanhoPagina, unsigned int tamanhoTotal, TrocaDePagina trocaDePagina, unsigned int quantum, Prioridade prioridade);
    ~SO();

    void Executa();
    void CarregarPrograma(std::string arquivo, unsigned int tempo);

    bool InstrucaoIlegal();
    bool ViolacaoDeMemoria();
    void InterrupcaoTemporizador(std::string interrupcao);

private:
    void AlterarTabela(unsigned int processo, unsigned int pagina);
    void AlterarQuadro(unsigned int quadro, unsigned int processo, unsigned int pagina);
    unsigned int ProximoQuadro();
    void EncerrarProcesso();

    void ContabilizarTempo();
    void Escalonar();
    void ImprimirRelatorio();

private:
    Memoria memoriaFisica;
    MMU mmu;
    CPU cpu;

    Controlador controlador;
    Temporizador temporizador;
    Escalonador escalonador;

    std::vector<DescritorQuadro> mapa;
    std::deque<unsigned int> quadrosFila;
    std::vector<DescritorJob*> jobs;
    std::vector<DescritorProcesso*> processos;
    int processoAtual;
    unsigned int processosExecutando;

    TrocaDePagina trocaDePagina;
    unsigned int quantum;
    Prioridade prioridade;

    unsigned int tempoAtivo = 0;
    unsigned int tempoOcioso = 0;
    unsigned int numFalhaPagina = 0;
    unsigned int qntExecutado = 0;
    unsigned int qntInstrucaoIlegal = 0;
    unsigned int qntViolacaoDeMemoria = 0;
    unsigned int qntInterrupcaoTemporizador = 0;
    unsigned int qntTrocaProcesso = 0;
    unsigned int qntPreempcao = 0;
};
