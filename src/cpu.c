#include "cpu.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct cpu_estado_t
{
    int pc;
    int acumulador;
    cpu_interrupcao_t interrupcao;
};

struct cpu
{
    cpu_estado_t estado;
    int programTam;
    char **programa;
    int dadosTam;
    int *dados;
};

void cpu_altera_programa(cpu *c, int tam, char **m)
{
    c->programTam = tam;
    c->programa = m;
}

void cpu_altera_dados(cpu* c, int tam, int *m)
{
    c->dadosTam = tam;
    c->dados = m;
}

cpu_interrupcao_t cpu_interrupcao(cpu *c)
{
    return c->estado.interrupcao;
}

void cpu_retorna_interrupcao(cpu *c)
{
    if (c->estado.interrupcao == normal)
        return;
    c->estado.interrupcao = normal;
    c->estado.pc++;
}

char *cpu_instrucao(cpu *c)
{
    if (c->estado.pc < 0 || c->estado.pc >= c->programTam)
    {
        c->estado.interrupcao = violacao_de_memoria;
        return NULL;
    }
    return c->programa[c->estado.pc];
}

void cpu_salva_estado(cpu *c, cpu_estado_t *e)
{
    *e = c->estado;
}

void cpu_recupera_estado(cpu *c, cpu_estado_t *e)
{
    c->estado = *e;
}

void cpu_estado_inicializa(cpu_estado_t *e)
{
    e->pc = 0;
    e->acumulador = 0;
    e->interrupcao = normal;
}

int memoria_ler(cpu *c, int i)
{
    if (i < 0 || i >= c->dadosTam)
    {
        c->estado.interrupcao = violacao_de_memoria;
        return 0;
    }
    return c->dados[i];
}

void memoria_escrever(cpu *c, int i, int valor)
{
    if (i < 0 || i >= c->dadosTam)
    {
        c->estado.interrupcao = violacao_de_memoria;
        return;
    }
    c->dados[i] = valor;
}

void cpu_executa(cpu *c)
{
    if (c->estado.interrupcao != normal)
        return;

    char *instrucao = cpu_instrucao(c);
    if (instrucao == NULL)
        return;

    //printf("PC = %2i | %s\n", c->estado.pc, instrucao);
    bool incrementar = true;

    if (strstr(instrucao, "CARGI"))
    {
        //CARGI	n	coloca o valor n no acumulador (A=n)
        int n;
        sscanf(instrucao, "%*s %i", &n);
        c->estado.acumulador = n;
        //printf("A=%i\n", n);
    }
    else if (strstr(instrucao, "CARGM"))
    {
        //CARGM	n	coloca no acumulador o valor na posição n da memória de dados (A=M[n])
        int n;
        sscanf(instrucao, "%*s %i", &n);
        c->estado.acumulador = memoria_ler(c, n);
        //c->dados[n];
        //printf("A=M[%i]\n", n);
    }
    else if (strstr(instrucao, "CARGX"))
    {
        //CARGX	n	coloca no acumulador o valor na posição que está na posição n da memória de dados (A=M[M[n]])
        int n;
        sscanf(instrucao, "%*s %i", &n);
        c->estado.acumulador = memoria_ler(c, memoria_ler(c, n));
        //c->dados[c->dados[n]];
        //printf("A=M[M[%i]]\n", n);
    }
    else if (strstr(instrucao, "ARMM"))
    {
        //ARMM	n	coloca o valor do acumulador na posição n da memória de dados (M[n]=A)
        int n;
        sscanf(instrucao, "%*s %i", &n);
        memoria_escrever(c, n, c->estado.acumulador);
        //c->dados[n] = c->estado.acumulador;
        //printf("M[%i]=A\n", n);
    }
    else if (strstr(instrucao, "ARMX"))
    {
        //ARMX	n	coloca o valor do acumulador posição que está na posição n da memória de dados (M[M[n]]=A)
        int n;
        sscanf(instrucao, "%*s %i", &n);
        memoria_escrever(c, memoria_ler(c, n), c->estado.acumulador);
        c->dados[c->dados[n]] = c->estado.acumulador;
        //printf("M[M[%i]]=A\n", n);
    }
    else if (strstr(instrucao, "SOMA"))
    {
        //SOMA	n	soma ao acumulador o valor no endereço n da memória de dados (A=A+M[n])
        int n;
        sscanf(instrucao, "%*s %i", &n);
        c->estado.acumulador += memoria_ler(c, n);
        //c->estado.acumulador += c->dados[n];
        //printf("A=A+M[%i]\n", n);
    }
    else if (strstr(instrucao, "NEG"))
    {
        //NEG		inverte o sinal do acumulador (A=-A)
        c->estado.acumulador = -c->estado.acumulador;
        //printf("A=-A\n");
    }
    else if (strstr(instrucao, "DESVZ"))
    {
        //DESVZ	n	se A vale 0, coloca o valor n no PC
        int n;
        sscanf(instrucao, "%*s %i", &n);
        if (c->estado.acumulador == 0)
        {
            c->estado.pc = n;
            incrementar = false;
        }
        //printf("if (A == 0) PC=%i\n", n);
    }
    else
    {
        //outra		coloca a CPU em interrupção – instrução ilegal
        c->estado.interrupcao = instrucao_ilegal;
        incrementar = false;
    }
    //printf("\n");

    if (incrementar && c->estado.interrupcao != violacao_de_memoria)
        c->estado.pc++;
}

cpu *cpu_cria()
{
    cpu *c = malloc(sizeof(cpu));
    cpu_estado_inicializa(&c->estado);
    return c;
}

void cpu_deleta(cpu *c)
{
    free(c);
}

void cpu_print(cpu *c)
{
    printf("CPU:\n");
    printf(" PC = %i\n", c->estado.pc);
    printf(" A = %i\n", c->estado.acumulador);
    printf(" Estado = ");
    switch (c->estado.interrupcao)
    {
        case normal:
            printf("Normal\n");
            break;
        case instrucao_ilegal:
            printf("Instrucao ilegal\n");
            break;
        case violacao_de_memoria:
            printf("Violacao de memoria\n");
            break;
    }
    printf(" Memoria de dados:\n");
    for (int i = 0; i < c->dadosTam; i++)
        printf("  %X = %i\n", i, c->dados[i]);
}
