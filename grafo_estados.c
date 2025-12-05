#include "grafo_estados.h"
#include "tipos.h"

// Array para mapear o enum para strings (para exibição)
const char* NOME_ESTADOS[MAX_ESTADOS] = {
    "Aguardando triagem",
    "Triado - Verde",
    "Triado - Amarelo",
    "Triado - Vermelho",
    "Em atendimento",
    "Em observacao",
    "Liberado",
    "Encaminhado para internacao"
};

// Funcao auxiliar para criar um novo no de adjacencia
NoAdj* criar_no_adj(EstadoClinico destino) {
    NoAdj *novo_no = (NoAdj*) malloc(sizeof(NoAdj));
    if (novo_no == NULL) {
        perror("Erro ao alocar memoria para NoAdj");
        exit(EXIT_FAILURE);
    }
    novo_no->destino = destino;
    novo_no->proximo = NULL;
    return novo_no;
}

// Funcao para criar e inicializar o grafo
Grafo* criar_grafo() {
    Grafo *grafo = (Grafo*) malloc(sizeof(Grafo));
    if (grafo == NULL) {
        perror("Erro ao alocar memoria para Grafo");
        exit(EXIT_FAILURE);
    }
    grafo->num_vertices = MAX_ESTADOS;
    grafo->lista_adjacencia = (NoAdj**) malloc(MAX_ESTADOS * sizeof(NoAdj*));
    if (grafo->lista_adjacencia == NULL) {
        perror("Erro ao alocar memoria para lista_adjacencia");
        free(grafo);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < MAX_ESTADOS; i++) {
        grafo->lista_adjacencia[i] = NULL;
    }

    // Definicao das arestas (transicoes permitidas)
    // 1. AGUARDANDO_TRIAGEM -> TRIADO_VERDE, TRIADO_AMARELO, TRIADO_VERMELHO
    adicionar_aresta(grafo, AGUARDANDO_TRIAGEM, TRIADO_VERDE);
    adicionar_aresta(grafo, AGUARDANDO_TRIAGEM, TRIADO_AMARELO);
    adicionar_aresta(grafo, AGUARDANDO_TRIAGEM, TRIADO_VERMELHO);

    // 2. TRIADO_VERDE -> EM_ATENDIMENTO
    adicionar_aresta(grafo, TRIADO_VERDE, EM_ATENDIMENTO);

    // 3. TRIADO_AMARELO -> EM_ATENDIMENTO
    adicionar_aresta(grafo, TRIADO_AMARELO, EM_ATENDIMENTO);

    // 4. TRIADO_VERMELHO -> EM_ATENDIMENTO
    adicionar_aresta(grafo, TRIADO_VERMELHO, EM_ATENDIMENTO);

    // 5. EM_ATENDIMENTO -> EM_OBSERVACAO, LIBERADO, ENCAMINHADO_INTERNACAO
    adicionar_aresta(grafo, EM_ATENDIMENTO, EM_OBSERVACAO);
    adicionar_aresta(grafo, EM_ATENDIMENTO, LIBERADO);
    adicionar_aresta(grafo, EM_ATENDIMENTO, ENCAMINHADO_INTERNACAO);

    // 6. EM_OBSERVACAO -> EM_ATENDIMENTO, LIBERADO, ENCAMINHADO_INTERNACAO
    adicionar_aresta(grafo, EM_OBSERVACAO, EM_ATENDIMENTO);
    adicionar_aresta(grafo, EM_OBSERVACAO, LIBERADO);
    adicionar_aresta(grafo, EM_OBSERVACAO, ENCAMINHADO_INTERNACAO);

    return grafo;
}

// Funcao para adicionar uma aresta (direcionada) ao grafo
void adicionar_aresta(Grafo *grafo, EstadoClinico origem, EstadoClinico destino) {
    if (origem >= MAX_ESTADOS || destino >= MAX_ESTADOS) {
        fprintf(stderr, "Erro: Estado de origem ou destino invalido.\n");
        return;
    }

    // Adiciona o destino a lista de adjacencia da origem
    NoAdj *novo_no = criar_no_adj(destino);
    novo_no->proximo = grafo->lista_adjacencia[origem];
    grafo->lista_adjacencia[origem] = novo_no;
}

// Funcao para liberar a memoria alocada para o grafo
void liberar_grafo(Grafo *grafo) {
    if (grafo == NULL) return;

    for (int i = 0; i < grafo->num_vertices; i++) {
        NoAdj *atual = grafo->lista_adjacencia[i];
        while (atual != NULL) {
            NoAdj *temp = atual;
            atual = atual->proximo;
            free(temp);
        }
    }
    free(grafo->lista_adjacencia);
    free(grafo);
}

// Funcao para exibir os vizinhos diretos de um estado (transicoes permitidas)
void exibir_vizinhos(Grafo *grafo, EstadoClinico estado) {
    if (estado >= MAX_ESTADOS) {
        printf("Estado invalido.\n");
        return;
    }

    printf("\n--- Possiveis Proximos Estados a partir de \"%s\" ---\n", NOME_ESTADOS[estado]);
    NoAdj *atual = grafo->lista_adjacencia[estado];
    if (atual == NULL) {
        printf("Nenhuma transicao permitida a partir deste estado.\n");
        return;
    }

    while (atual != NULL) {
        printf("  -> %s\n", NOME_ESTADOS[atual->destino]);
        atual = atual->proximo;
    }
    printf("----------------------------------------------------------------------\n");
}

// Funcao auxiliar para a Busca em Profundidade (DFS)
void dfs_recursiva(Grafo *grafo, EstadoClinico u, int *visitado) {
    visitado[u] = 1;
    printf("  -> %s\n", NOME_ESTADOS[u]);

    NoAdj *atual = grafo->lista_adjacencia[u];
    while (atual != NULL) {
        EstadoClinico v = atual->destino;
        if (!visitado[v]) {
            dfs_recursiva(grafo, v, visitado);
        }
        atual = atual->proximo;
    }
}

// Funcao para realizar a Busca em Profundidade (DFS)
void busca_em_profundidade(Grafo *grafo, EstadoClinico inicio) {
    if (inicio >= MAX_ESTADOS) {
        printf("Estado inicial invalido.\n");
        return;
    }

    int visitado[MAX_ESTADOS];
    for (int i = 0; i < MAX_ESTADOS; i++) {
        visitado[i] = 0;
    }

    printf("\n--- Estados Alcançaveis a partir de \"%s\" (DFS) ---\n", NOME_ESTADOS[inicio]);
    dfs_recursiva(grafo, inicio, visitado);
    printf("----------------------------------------------------------------------\n");
}

// Funcao para realizar a Busca em Largura (BFS)
void busca_em_largura(Grafo *grafo, EstadoClinico inicio) {
    if (inicio >= MAX_ESTADOS) {
        printf("Estado inicial invalido.\n");
        return;
    }

    int visitado[MAX_ESTADOS];
    for (int i = 0; i < MAX_ESTADOS; i++) {
        visitado[i] = 0;
    }

    // Implementacao simples de fila para BFS (usando array circular ou lista)
    // Para simplificar, usaremos um array como fila
    EstadoClinico fila[MAX_ESTADOS];
    int frente = 0;
    int tras = 0;

    printf("\n--- Estados Alcançaveis a partir de \"%s\" (BFS) ---\n", NOME_ESTADOS[inicio]);

    // Enfileira o estado inicial e marca como visitado
    visitado[inicio] = 1;
    fila[tras++] = inicio;
    printf("  -> %s\n", NOME_ESTADOS[inicio]);

    while (frente != tras) {
        // Desenfileira um vertice
        EstadoClinico u = fila[frente++];

        // Percorre todos os vizinhos do vertice desenfileirado
        NoAdj *atual = grafo->lista_adjacencia[u];
        while (atual != NULL) {
            EstadoClinico v = atual->destino;
            if (!visitado[v]) {
                visitado[v] = 1;
                printf("  -> %s\n", NOME_ESTADOS[v]);
                // Enfileira o vizinho
                if (tras < MAX_ESTADOS) {
                    fila[tras++] = v;
                }
            }
            atual = atual->proximo;
        }
    }
    printf("----------------------------------------------------------------------\n");
}
