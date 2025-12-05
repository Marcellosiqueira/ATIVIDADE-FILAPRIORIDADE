#ifndef GRAFO_ESTADOS_H
#define GRAFO_ESTADOS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tipos.h"

#define MAX_ESTADOS 8

// Estrutura para um nó da lista de adjacência
typedef struct NoAdj {
    EstadoClinico destino;
    struct NoAdj *proximo;
} NoAdj;

// Estrutura para o Grafo (Lista de Adjacência)
typedef struct Grafo {
    int num_vertices;
    NoAdj **lista_adjacencia;
} Grafo;

// Array para mapear o enum para strings (para exibição)
extern const char* NOME_ESTADOS[MAX_ESTADOS];

// Funções do Grafo
Grafo* criar_grafo();
void adicionar_aresta(Grafo *grafo, EstadoClinico origem, EstadoClinico destino);
void liberar_grafo(Grafo *grafo);

// Funções de Busca
void exibir_vizinhos(Grafo *grafo, EstadoClinico estado);
void busca_em_largura(Grafo *grafo, EstadoClinico inicio);
void busca_em_profundidade(Grafo *grafo, EstadoClinico inicio);

#endif // GRAFO_ESTADOS_H
