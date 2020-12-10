#include <iostream>

#include "so.h"

int main(int argc, char *argv[])
{
    SO so;
    so.CarregarDispositivo("dispositivo_0.txt");
    so.CarregarDispositivo("dispositivo_1.txt");
    so.CarregarDispositivo("dispositivo_2.txt");
    so.CarregarDispositivo("dispositivo_3.txt");
    so.CarregarJob("programa_mult.txt");
    so.CarregarJob("programa_fatorial.txt");
    so.Executa();
    return 0;
}
