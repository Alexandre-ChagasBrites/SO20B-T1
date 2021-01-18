#include "so.h"

int main(int argc, char *argv[])
{
    SO so(10, SO::Prioridade::Abaixo);
    so.CarregarPrograma("programa_es.txt", 10);
    so.CarregarPrograma("programa_cpu.txt", 20);
    so.CarregarPrograma("programa_es.txt", 30);
    so.CarregarPrograma("programa_cpu.txt", 40);
    so.CarregarPrograma("programa_cpu.txt", 50);
    so.CarregarPrograma("programa_cpu.txt", 60);
    so.CarregarPrograma("programa_es.txt", 70);
    so.CarregarPrograma("programa_cpu.txt", 80);
    so.CarregarPrograma("programa_es.txt", 90);
    so.CarregarPrograma("programa_es.txt", 100);
    so.Executa();
    return 0;
}
