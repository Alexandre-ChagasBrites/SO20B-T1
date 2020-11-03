#pragma once

typedef struct cpu cpu;
typedef struct cpu_estado_t cpu_estado_t;
typedef enum { normal, instrucao_ilegal, violacao_de_memoria } cpu_interrupcao_t;

void cpu_altera_programa(cpu *c, int tam, char **m);
void cpu_altera_dados(cpu* c, int tam, int *m);
cpu_interrupcao_t cpu_interrupcao(cpu *c);
void cpu_retorna_interrupcao(cpu *c);
char *cpu_instrucao(cpu *c);
void cpu_salva_estado(cpu *c, cpu_estado_t *e);
void cpu_recupera_estado(cpu *c, cpu_estado_t *e);
void cpu_estado_inicializa(cpu_estado_t *e);
void cpu_executa(cpu *c);

cpu *cpu_cria();
void cpu_deleta(cpu *c);
void cpu_print(cpu *c);
