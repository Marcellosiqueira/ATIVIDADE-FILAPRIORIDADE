#ifndef HEAP_PRIORIDADE_H
#define HEAP_PRIORIDADE_H

#include "tipos.h"

#define CAPACIDADE_INICIAL 100

// Estrutura para o Heap Binário (Fila de Prioridade)
typedef struct HeapPrioridade {
    Paciente *elementos;
    int tamanho;
    int capacidade;
    // Variaveis para estatisticas
    int total_atendidos;
    int atendidos_por_prioridade[3]; // Indice 0: Vermelho, 1: Amarelo, 2: Verde
    double tempo_total_espera_por_prioridade[3];
} HeapPrioridade;

// Funcoes do Heap
HeapPrioridade* inicializar_heap();
int heap_vazio(HeapPrioridade *heap);
int inserir_heap(HeapPrioridade *heap, Paciente paciente);
int remover_max_heap(HeapPrioridade *heap, Paciente *paciente_removido);
int consultar_max_heap(HeapPrioridade *heap, Paciente *paciente_consultado);
void exibir_heap(HeapPrioridade *heap);
void liberar_heap(HeapPrioridade *heap);

// Funcoes de Estatisticas
void exibir_estatisticas(HeapPrioridade *heap);

#endif // HEAP_PRIORIDADE_H
