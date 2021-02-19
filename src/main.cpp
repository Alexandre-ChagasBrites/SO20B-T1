#include "so.h"

int main(int argc, char *argv[])
{
    SO so(4, 10, SO::TrocaDePagina::FIFO, 5, SO::Prioridade::Abaixo);
    so.CarregarPrograma("arquivos_escalonador/programa_es.txt", 10);
    so.CarregarPrograma("arquivos_escalonador/programa_cpu.txt", 20);
    so.CarregarPrograma("arquivos_escalonador/programa_es.txt", 30);
    so.CarregarPrograma("arquivos_escalonador/programa_cpu.txt", 40);
    so.CarregarPrograma("arquivos_escalonador/programa_cpu.txt", 50);
    so.CarregarPrograma("arquivos_escalonador/programa_cpu.txt", 60);
    so.CarregarPrograma("arquivos_escalonador/programa_es.txt", 70);
    so.CarregarPrograma("arquivos_escalonador/programa_cpu.txt", 80);
    so.CarregarPrograma("arquivos_escalonador/programa_es.txt", 90);
    so.CarregarPrograma("arquivos_escalonador/programa_es.txt", 100);
    so.Executa();
    return 0;
}
