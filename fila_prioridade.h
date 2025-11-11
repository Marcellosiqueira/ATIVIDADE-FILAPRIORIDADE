#ifndef FILA_PRIORIDADE_H
#define FILA_PRIORIDADE_H


typedef struct Paciente {
    char nome[50];
    int prioridade; // 1 (vermelho - emergencia), 2 (amarelo - urgencia), 3 (verde - nao urgente)
} Paciente;


typedef struct No {
    Paciente paciente;
    struct No *proximo;
} No;


typedef struct FilaPrioridade {
    No *inicio;
} FilaPrioridade;

FilaPrioridade* inicializar_fila();

int fila_vazia(FilaPrioridade *fila);

int enfileirar_prioridade(FilaPrioridade *fila, Paciente paciente);

int desenfileirar_prioridade(FilaPrioridade *fila, Paciente *paciente_removido);

int consultar_frente(FilaPrioridade *fila, Paciente *paciente_consultado);

void exibir_fila(FilaPrioridade *fila);

void liberar_fila(FilaPrioridade *fila);

#endif
