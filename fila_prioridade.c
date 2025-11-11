#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fila_prioridade.h"

FilaPrioridade* inicializar_fila() {
    FilaPrioridade *nova_fila = (FilaPrioridade*) malloc(sizeof(FilaPrioridade));
    if (nova_fila == NULL) {
        perror("Erro ao alocar memoria para a fila");
        return NULL;
    }
    nova_fila->inicio = NULL;
    return nova_fila;
}

int fila_vazia(FilaPrioridade *fila) {
    if (fila == NULL) {
        return 1;
    }
    return fila->inicio == NULL;
}

int enfileirar_prioridade(FilaPrioridade *fila, Paciente paciente) {
    if (fila == NULL) {
        return 0;
    }

    No *novo_no = (No*) malloc(sizeof(No));
    if (novo_no == NULL) {
        perror("Erro ao alocar memoria para o no");
        return 0;
    }
    novo_no->paciente = paciente;
    novo_no->proximo = NULL;

    if (fila_vazia(fila) || paciente.prioridade < fila->inicio->paciente.prioridade) {
        novo_no->proximo = fila->inicio;
        fila->inicio = novo_no;
        return 1;
    }
    No *atual = fila->inicio;
    while (atual->proximo != NULL && atual->proximo->paciente.prioridade <= paciente.prioridade) {
        atual = atual->proximo;
    }
    novo_no->proximo = atual->proximo;
    atual->proximo = novo_no;

    return 1;
}

int desenfileirar_prioridade(FilaPrioridade *fila, Paciente *paciente_removido) {
    if (fila_vazia(fila)) {
        return 0;
    }

    No *no_removido = fila->inicio;
    *paciente_removido = no_removido->paciente;
    fila->inicio = no_removido->proximo;

    free(no_removido);
    return 1;
}

int consultar_frente(FilaPrioridade *fila, Paciente *paciente_consultado) {
    if (fila_vazia(fila)) {
        return 0;
    }

    *paciente_consultado = fila->inicio->paciente;
    return 1;
}

void exibir_fila(FilaPrioridade *fila) {
    if (fila_vazia(fila)) {
        printf("A fila de pacientes esta vazia.\n");
        return;
    }

    printf("\n--- FILA DE ATENDIMENTO (Prioridade: 1=Vermelho, 2=Amarelo, 3=Verde) ---\n");
    No *atual = fila->inicio;
    while (atual != NULL) {
        printf("  -> Paciente: %s (Prioridade: %d)\n", atual->paciente.nome, atual->paciente.prioridade);
        atual = atual->proximo;
    }
    printf("----------------------------------------------------------------------\n");
}

void liberar_fila(FilaPrioridade *fila) {
    if (fila == NULL) {
        return;
    }

    No *atual = fila->inicio;
    No *proximo;

    while (atual != NULL) {
        proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }

    free(fila);
}
