#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "cpu.h"

#define DADOS_TAM 8

char **programa_carrega(char *path, int *tam)
{
    FILE *file = fopen(path, "rb");
    fseek(file, 0, SEEK_END);
    int len = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *text = malloc((len + 1) * sizeof(char));
    fread(text, len, sizeof(char), file);
    text[len] = 0;
    fclose(file);

    *tam = 1;
    char *c = text;
    while (*c != '\0')
    {
        if (*(c++) == '\n')
            (*tam)++;
    }

    char **programa = malloc((*tam) * sizeof(char *));
    int i = 0;

    c = strtok(text, "\n");
    while (c != NULL)
    {
        programa[i] = calloc((strlen(c) + 1), sizeof(char));
        strcpy(programa[i++], c);
        c = strtok(NULL, "\n");
    }

    return programa;
}

int main(int argc, char *argv[])
{
    int programaTam;
    char **programa = programa_carrega("programa.txt", &programaTam);

    int dados[DADOS_TAM];
    memset(dados, 0, sizeof(dados));

    cpu *c = cpu_cria();
    cpu_altera_programa(c, programaTam, programa);
    cpu_altera_dados(c, DADOS_TAM, dados);

    while(true)
    {
        cpu_interrupcao_t interrupcao = cpu_interrupcao(c);
        if (interrupcao != normal)
            break;

        cpu_executa(c);
    }
    cpu_print(c);

    cpu_deleta(c);
    free(programa);

    return 0;
}
