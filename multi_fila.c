#include "multi_fila.h"
#include "grafo_estados.h" // Para usar NOME_ESTADOS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Funcao auxiliar para criar um novo no
No* criar_no(Paciente paciente) {
    No *novo_no = (No*) malloc(sizeof(No));
    if (novo_no == NULL) {
        perror("Erro ao alocar memoria para No");
        return NULL;
    }
    novo_no->paciente = paciente;
    novo_no->proximo = NULL;
    return novo_no;
}

// Funcao para criar e inicializar a MultiFila
MultiFila* inicializar_multi_fila() {
    MultiFila *nova_mf = (MultiFila*) malloc(sizeof(MultiFila));
    if (nova_mf == NULL) {
        perror("Erro ao alocar memoria para MultiFila");
        return NULL;
    }
    for (int i = 0; i < 3; i++) {
        nova_mf->filas[i] = NULL;
        nova_mf->atendidos_por_prioridade[i] = 0;
        nova_mf->tempo_total_espera_por_prioridade[i] = 0.0;
    }
    nova_mf->total_atendidos = 0;
    return nova_mf;
}

// Funcao para verificar se a MultiFila esta vazia
int multi_fila_vazia(MultiFila *mf) {
    if (mf == NULL) {
        return 1;
    }
    for (int i = 0; i < 3; i++) {
        if (mf->filas[i] != NULL) {
            return 0;
        }
    }
    return 1;
}

// Funcao para enfileirar um paciente na fila apropriada (com criterio de desempate)
int enfileirar_multi_fila(MultiFila *mf, Paciente paciente) {
    if (mf == NULL) {
        return 0;
    }

    int indice = paciente.prioridade - 1; // 1->0, 2->1, 3->2
    if (indice < 0 || indice > 2) {
        fprintf(stderr, "Erro: Prioridade invalida para MultiFila.\n");
        return 0;
    }

    No *novo_no = criar_no(paciente);
    if (novo_no == NULL) {
        return 0;
    }

    // A insercao e feita no final da fila (FIFO) para pacientes da mesma prioridade.
    // Isso implementa o criterio de desempate por tempo de chegada (Sugestao 8).
    if (mf->filas[indice] == NULL) {
        mf->filas[indice] = novo_no;
    } else {
        No *atual = mf->filas[indice];
        while (atual->proximo != NULL) {
            atual = atual->proximo;
        }
        atual->proximo = novo_no;
    }

    return 1;
}

// Funcao para desenfileirar o paciente de maior prioridade
int desenfileirar_multi_fila(MultiFila *mf, Paciente *paciente_removido) {
    if (multi_fila_vazia(mf)) {
        return 0;
    }
    if (paciente_removido == NULL) {
        return 0;
    }

    // Procura a fila de maior prioridade (indice 0, 1, 2) que nao esta vazia
    int indice_maior_prioridade = -1;
    for (int i = 0; i < 3; i++) {
        if (mf->filas[i] != NULL) {
            indice_maior_prioridade = i;
            break;
        }
    }

    if (indice_maior_prioridade == -1) {
        return 0; // Nao deveria acontecer se multi_fila_vazia() for false
    }

    No *no_removido = mf->filas[indice_maior_prioridade];
    *paciente_removido = no_removido->paciente;
    mf->filas[indice_maior_prioridade] = no_removido->proximo;

    // Atualiza estatisticas
    time_t hora_atendimento = time(NULL);
    double tempo_espera = difftime(hora_atendimento, paciente_removido->hora_chegada);

    mf->total_atendidos++;
    mf->atendidos_por_prioridade[indice_maior_prioridade]++;
    mf->tempo_total_espera_por_prioridade[indice_maior_prioridade] += tempo_espera;

    free(no_removido);
    return 1;
}

// Funcao para consultar o paciente de maior prioridade
int consultar_frente_multi_fila(MultiFila *mf, Paciente *paciente_consultado) {
    if (multi_fila_vazia(mf)) {
        return 0;
    }
    if (paciente_consultado == NULL) {
        return 0;
    }

    // Procura a fila de maior prioridade (indice 0, 1, 2) que nao esta vazia
    for (int i = 0; i < 3; i++) {
        if (mf->filas[i] != NULL) {
            *paciente_consultado = mf->filas[i]->paciente;
            return 1;
        }
    }

    return 0;
}

// Funcao para exibir a MultiFila
void exibir_multi_fila(MultiFila *mf) {
    if (multi_fila_vazia(mf)) {
        printf("A fila de pacientes esta vazia.\n");
        return;
    }

    printf("\n--- FILA DE ATENDIMENTO (Multiplas Filas) ---\n");
    const char* cores[] = {"Vermelho (Emergencia)", "Amarelo (Urgencia)", "Verde (Nao Urgente)"};

    for (int i = 0; i < 3; i++) {
        printf("\n[%s]\n", cores[i]);
        No *atual = mf->filas[i];
        if (atual == NULL) {
            printf("  (Vazia)\n");
            continue;
        }
        while (atual != NULL) {
            printf("  -> Paciente: %s (Prioridade: %d, Estado: %s)\n", atual->paciente.nome, atual->paciente.prioridade, NOME_ESTADOS[atual->paciente.estado_atual]);
            atual = atual->proximo;
        }
    }
    printf("----------------------------------------------------------------------\n");
}

// Funcao para liberar a memoria alocada para a MultiFila
void liberar_multi_fila(MultiFila *mf) {
    if (mf == NULL) return;

    for (int i = 0; i < 3; i++) {
        No *atual = mf->filas[i];
        while (atual != NULL) {
            No *temp = atual;
            atual = atual->proximo;
            free(temp);
        }
    }
    free(mf);
}

// Funcao para exibir o relatorio estatistico da MultiFila
void exibir_estatisticas_multi_fila(MultiFila *mf) {
    if (mf == NULL) {
        printf("\n[ERRO] Estrutura de dados nao inicializada.\n");
        return;
    }

    printf("\n--- RELATORIO ESTATISTICO DE ATENDIMENTO (MultiFila) ---\n");
    printf("Total de pacientes atendidos: %d\n", mf->total_atendidos);

    const char* cores[] = {"Vermelho (Emergencia)", "Amarelo (Urgencia)", "Verde (Nao Urgente)"};

    for (int i = 0; i < 3; i++) {
        int atendidos = mf->atendidos_por_prioridade[i];
        double tempo_total = mf->tempo_total_espera_por_prioridade[i];
        double tempo_medio = (atendidos > 0) ? tempo_total / atendidos : 0.0;

        printf("\n[%s]\n", cores[i]);
        printf("  - Pacientes atendidos: %d\n", atendidos);
        printf("  - Tempo total de espera: %.2f segundos\n", tempo_total);
        printf("  - Tempo medio de espera: %.2f segundos\n", tempo_medio);
    }
    printf("----------------------------------------------------------------------\n");
}
