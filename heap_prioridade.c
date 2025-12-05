#include "heap_prioridade.h"
#include "grafo_estados.h" // Para usar NOME_ESTADOS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Funcao auxiliar para trocar dois pacientes no heap
void trocar_pacientes(Paciente *a, Paciente *b) {
    Paciente temp = *a;
    *a = *b;
    *b = temp;
}

// Funcao para manter a propriedade do Max-Heap (prioridade 1 > 2 > 3)
void max_heapify(HeapPrioridade *heap, int i) {
    int maior = i;
    int esquerda = 2 * i + 1;
    int direita = 2 * i + 2;

    // A prioridade mais baixa (1) tem a maior importancia
    if (esquerda < heap->tamanho && heap->elementos[esquerda].prioridade < heap->elementos[maior].prioridade) {
        maior = esquerda;
    }
    if (direita < heap->tamanho && heap->elementos[direita].prioridade < heap->elementos[maior].prioridade) {
        maior = direita;
    }

    if (maior != i) {
        trocar_pacientes(&heap->elementos[i], &heap->elementos[maior]);
        max_heapify(heap, maior);
    }
}

// Funcao para ajustar o heap apos a insercao
void subir_heap(HeapPrioridade *heap, int i) {
    int pai = (i - 1) / 2;
    // A prioridade mais baixa (1) tem a maior importancia
    while (i != 0 && heap->elementos[i].prioridade < heap->elementos[pai].prioridade) {
        trocar_pacientes(&heap->elementos[i], &heap->elementos[pai]);
        i = pai;
        pai = (i - 1) / 2;
    }
}

// Funcao para redimensionar o array do heap
int redimensionar_heap(HeapPrioridade *heap) {
    int nova_capacidade = heap->capacidade * 2;
    Paciente *novos_elementos = (Paciente*) realloc(heap->elementos, nova_capacidade * sizeof(Paciente));
    if (novos_elementos == NULL) {
        perror("Erro ao redimensionar o heap");
        return 0;
    }
    heap->elementos = novos_elementos;
    heap->capacidade = nova_capacidade;
    return 1;
}

// Funcao para criar e inicializar o heap
HeapPrioridade* inicializar_heap() {
    HeapPrioridade *novo_heap = (HeapPrioridade*) malloc(sizeof(HeapPrioridade));
    if (novo_heap == NULL) {
        perror("Erro ao alocar memoria para o heap");
        return NULL;
    }
    novo_heap->elementos = (Paciente*) malloc(CAPACIDADE_INICIAL * sizeof(Paciente));
    if (novo_heap->elementos == NULL) {
        perror("Erro ao alocar memoria para elementos do heap");
        free(novo_heap);
        return NULL;
    }
    novo_heap->tamanho = 0;
    novo_heap->capacidade = CAPACIDADE_INICIAL;
    novo_heap->total_atendidos = 0;
    for (int i = 0; i < 3; i++) {
        novo_heap->atendidos_por_prioridade[i] = 0;
        novo_heap->tempo_total_espera_por_prioridade[i] = 0.0;
    }
    return novo_heap;
}

// Funcao para verificar se o heap esta vazio
int heap_vazio(HeapPrioridade *heap) {
    if (heap == NULL) {
        return 1;
    }
    return heap->tamanho == 0;
}

// Funcao para inserir um paciente no heap
int inserir_heap(HeapPrioridade *heap, Paciente paciente) {
    if (heap == NULL) {
        return 0;
    }
    if (heap->tamanho == heap->capacidade) {
        if (!redimensionar_heap(heap)) {
            return 0;
        }
    }

    // Define a hora de chegada
    paciente.hora_chegada = time(NULL);

    heap->elementos[heap->tamanho] = paciente;
    subir_heap(heap, heap->tamanho);
    heap->tamanho++;
    return 1;
}

// Funcao para remover o paciente de maior prioridade (raiz do heap)
int remover_max_heap(HeapPrioridade *heap, Paciente *paciente_removido) {
    if (heap_vazio(heap)) {
        return 0;
    }
    if (paciente_removido == NULL) {
        return 0; // Tratamento seguro de ponteiro
    }

    // Remove o paciente de maior prioridade (raiz)
    *paciente_removido = heap->elementos[0];

    // Atualiza estatisticas
    time_t hora_atendimento = time(NULL);
    double tempo_espera = difftime(hora_atendimento, paciente_removido->hora_chegada);
    int indice_prioridade = paciente_removido->prioridade - 1; // 1->0, 2->1, 3->2

    heap->total_atendidos++;
    heap->atendidos_por_prioridade[indice_prioridade]++;
    heap->tempo_total_espera_por_prioridade[indice_prioridade] += tempo_espera;

    // Move o ultimo elemento para a raiz e ajusta o heap
    heap->tamanho--;
    if (heap->tamanho > 0) {
        heap->elementos[0] = heap->elementos[heap->tamanho];
        max_heapify(heap, 0);
    }

    return 1;
}

// Funcao para consultar o paciente de maior prioridade (raiz do heap)
int consultar_max_heap(HeapPrioridade *heap, Paciente *paciente_consultado) {
    if (heap_vazio(heap)) {
        return 0;
    }
    if (paciente_consultado == NULL) {
        return 0; // Tratamento seguro de ponteiro
    }

    *paciente_consultado = heap->elementos[0];
    return 1;
}

// Funcao para exibir o heap (simplesmente percorre o array)
void exibir_heap(HeapPrioridade *heap) {
    if (heap_vazio(heap)) {
        printf("A fila de pacientes esta vazia.\n");
        return;
    }

    printf("\n--- FILA DE ATENDIMENTO (Prioridade: 1=Vermelho, 2=Amarelo, 3=Verde) ---\n");
    for (int i = 0; i < heap->tamanho; i++) {
        Paciente p = heap->elementos[i];
        printf("  -> Paciente: %s (Prioridade: %d, Estado: %s)\n", p.nome, p.prioridade, NOME_ESTADOS[p.estado_atual]);
    }
    printf("----------------------------------------------------------------------\n");
}

// Funcao para liberar a memoria alocada para o heap
void liberar_heap(HeapPrioridade *heap) {
    if (heap == NULL) return;
    if (heap->elementos != NULL) {
        free(heap->elementos);
    }
    free(heap);
}

// Funcao para exibir o relatorio estatistico
void exibir_estatisticas(HeapPrioridade *heap) {
    if (heap == NULL) {
        printf("\n[ERRO] Estrutura de dados nao inicializada.\n");
        return;
    }

    printf("\n--- RELATORIO ESTATISTICO DE ATENDIMENTO ---\n");
    printf("Total de pacientes atendidos: %d\n", heap->total_atendidos);

    const char* cores[] = {"Vermelho (Emergencia)", "Amarelo (Urgencia)", "Verde (Nao Urgente)"};

    for (int i = 0; i < 3; i++) {
        int atendidos = heap->atendidos_por_prioridade[i];
        double tempo_total = heap->tempo_total_espera_por_prioridade[i];
        double tempo_medio = (atendidos > 0) ? tempo_total / atendidos : 0.0;

        printf("\n[%s]\n", cores[i]);
        printf("  - Pacientes atendidos: %d\n", atendidos);
        printf("  - Tempo total de espera: %.2f segundos\n", tempo_total);
        printf("  - Tempo medio de espera: %.2f segundos\n", tempo_medio);
    }
    printf("----------------------------------------------------------------------\n");
}
