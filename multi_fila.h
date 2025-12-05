#ifndef MULTI_FILA_H
#define MULTI_FILA_H

#include "tipos.h"

// Estrutura para um nó da lista encadeada (fila)
typedef struct No {
    Paciente paciente;
    struct No *proximo;
} No;

// Estrutura para a Fila de Prioridade com Múltiplas Filas
typedef struct MultiFila {
    No *filas[3]; // 0: Vermelho, 1: Amarelo, 2: Verde
    // Variaveis para estatisticas
    int total_atendidos;
    int atendidos_por_prioridade[3]; // Indice 0: Vermelho, 1: Amarelo, 2: Verde
    double tempo_total_espera_por_prioridade[3];
} MultiFila;

// Funcoes da MultiFila
MultiFila* inicializar_multi_fila();
int multi_fila_vazia(MultiFila *mf);
int enfileirar_multi_fila(MultiFila *mf, Paciente paciente);
int desenfileirar_multi_fila(MultiFila *mf, Paciente *paciente_removido);
int consultar_frente_multi_fila(MultiFila *mf, Paciente *paciente_consultado);
void exibir_multi_fila(MultiFila *mf);
void liberar_multi_fila(MultiFila *mf);

// Funcoes de Estatisticas
void exibir_estatisticas_multi_fila(MultiFila *mf);

#endif // MULTI_FILA_H
