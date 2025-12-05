#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tipos.h"
#include "heap_prioridade.h"
#include "multi_fila.h"
#include "grafo_estados.h"

void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Funcao para simular a insercao de pacientes (Sugestao 10)
void simular_atendimento(HeapPrioridade *heap, MultiFila *mf, int num_pacientes, int tipo_fila) {
    printf("\n--- SIMULACAO DE ATENDIMENTO (%d pacientes) ---\n", num_pacientes);
    srand(time(NULL));

    for (int i = 0; i < num_pacientes; i++) {
        Paciente novo_paciente;
        char nome[50];
        sprintf(nome, "Paciente Simulado %d", i + 1);
        strcpy(novo_paciente.nome, nome);
        novo_paciente.prioridade = (rand() % 3) + 1; // 1, 2 ou 3
        novo_paciente.estado_atual = AGUARDANDO_TRIAGEM;
        novo_paciente.hora_chegada = time(NULL);

        if (tipo_fila == 1) {
            inserir_heap(heap, novo_paciente);
        } else {
            enfileirar_multi_fila(mf, novo_paciente);
        }
    }
    printf("[SUCESSO] %d pacientes simulados adicionados a fila.\n", num_pacientes);
}

int menu() {
    int escolha;
    printf("\n--- Sistema de Triagem de Pronto-Socorro ---\n");
    printf("1. Adicionar Novo Paciente (Triagem)\n");
    printf("2. Chamar Proximo Paciente (Atendimento)\n");
    printf("3. Consultar Proximo Paciente\n");
    printf("4. Exibir Fila de Espera\n");
    printf("5. Ver possiveis proximos estados (Vizinhos)\n");
    printf("6. Ver todos os estados alcancaveis (Busca)\n");
    printf("7. Exibir Relatorio Estatistico\n");
    printf("8. Simular Atendimento (Teste Automatico)\n");
    printf("9. Sair\n");
    printf("Escolha uma opcao: ");
    if (scanf("%d", &escolha) != 1) {
        limpar_buffer();
        return 0;
    }
    limpar_buffer();
    return escolha;
}

Paciente obter_dados_paciente() {
    Paciente novo_paciente;
    int prioridade_valida = 0;

    printf("\n--- Triagem de Paciente ---\n");
    // O estado inicial de todo paciente e "Aguardando triagem"
    novo_paciente.estado_atual = AGUARDANDO_TRIAGEM;
    printf("Nome do Paciente: ");
    fgets(novo_paciente.nome, 50, stdin);
    novo_paciente.nome[strcspn(novo_paciente.nome, "\n")] = 0;

    while (!prioridade_valida) {
        printf("Prioridade (1=Vermelho/Emergencia, 2=Amarelo/Urgencia, 3=Verde/Nao Urgente): ");
        if (scanf("%d", &novo_paciente.prioridade) != 1) {
            printf("Entrada invalida. Por favor, digite um numero.\n");
            limpar_buffer();
        } else if (novo_paciente.prioridade < 1 || novo_paciente.prioridade > 3) {
            printf("Prioridade invalida. Escolha 1, 2 ou 3.\n");
            limpar_buffer();
        } else {
            prioridade_valida = 1;
        }
    }
    limpar_buffer();

    // Define a hora de chegada aqui, pois a funcao inserir_heap nao a define mais
    novo_paciente.hora_chegada = time(NULL);

    return novo_paciente;
}

EstadoClinico selecionar_estado() {
    int escolha;
    int estado_valido = 0;

    printf("\n--- Selecao de Estado Clinico ---\n");
    for (int i = 0; i < MAX_ESTADOS; i++) {
        printf("%d. %s\n", i + 1, NOME_ESTADOS[i]);
    }

    while (!estado_valido) {
        printf("Escolha o estado inicial (1 a %d): ", MAX_ESTADOS);
        if (scanf("%d", &escolha) != 1) {
            printf("Entrada invalida. Por favor, digite um numero.\n");
            limpar_buffer();
        } else if (escolha < 1 || escolha > MAX_ESTADOS) {
            printf("Opcao invalida. Escolha um numero entre 1 e %d.\n", MAX_ESTADOS);
            limpar_buffer();
        } else {
            estado_valido = 1;
        }
    }
    limpar_buffer();

    return (EstadoClinico)(escolha - 1);
}

int main() {
    int tipo_fila = 0;
    while (tipo_fila < 1 || tipo_fila > 2) {
        printf("\n--- SELECAO DE IMPLEMENTACAO DA FILA DE PRIORIDADE ---\n");
        printf("1. Heap Binario (Sugestao 1 - Desafio)\n");
        printf("2. Multiplas Filas (Sugestao 9 - Didatico)\n");
        printf("Escolha a implementacao (1 ou 2): ");
        if (scanf("%d", &tipo_fila) != 1) {
            limpar_buffer();
            printf("\n[AVISO] Entrada invalida. Tente novamente.\n");
            tipo_fila = 0;
        } else if (tipo_fila < 1 || tipo_fila > 2) {
            limpar_buffer();
            printf("\n[AVISO] Opcao invalida. Escolha 1 ou 2.\n");
        }
    }
    limpar_buffer();

    HeapPrioridade *heap_fila = NULL;
    MultiFila *multi_fila = NULL;

    if (tipo_fila == 1) {
        heap_fila = inicializar_heap();
        if (heap_fila == NULL) {
            fprintf(stderr, "Falha ao inicializar o Heap Binario.\n");
            return 1;
        }
    } else {
        multi_fila = inicializar_multi_fila();
        if (multi_fila == NULL) {
            fprintf(stderr, "Falha ao inicializar as Multiplas Filas.\n");
            return 1;
        }
    }

    Grafo *fluxo_atendimento = criar_grafo();
    if (fluxo_atendimento == NULL) {
        fprintf(stderr, "Falha ao inicializar o grafo de fluxo.\n");
        if (heap_fila) liberar_heap(heap_fila);
        if (multi_fila) liberar_multi_fila(multi_fila);
        return 1;
    }

    int escolha;
    Paciente paciente_aux;

    do {
        escolha = menu();

        switch (escolha) {
            case 1: // Adicionar Novo Paciente
                paciente_aux = obter_dados_paciente();
                int sucesso = (tipo_fila == 1) ? inserir_heap(heap_fila, paciente_aux) : enfileirar_multi_fila(multi_fila, paciente_aux);
                if (sucesso) {
                    printf("\n[SUCESSO] Paciente %s adicionado a fila com prioridade %d. Estado inicial: %s.\n", paciente_aux.nome, paciente_aux.prioridade, NOME_ESTADOS[paciente_aux.estado_atual]);
                } else {
                    printf("\n[ERRO] Falha ao adicionar paciente a fila.\n");
                }
                break;

            case 2: // Chamar Proximo Paciente
                int removido = (tipo_fila == 1) ? remover_max_heap(heap_fila, &paciente_aux) : desenfileirar_multi_fila(multi_fila, &paciente_aux);
                if (removido) {
                    printf("\n[ATENDIMENTO] Chamando paciente: %s (Prioridade: %d - %s). Estado anterior: %s.\n", paciente_aux.nome, paciente_aux.prioridade, (paciente_aux.prioridade == 1 ? "Vermelho" : (paciente_aux.prioridade == 2 ? "Amarelo" : "Verde")), NOME_ESTADOS[paciente_aux.estado_atual]);
                } else {
                    printf("\n[INFO] A fila de pacientes esta vazia. Sem pacientes para atender.\n");
                }
                break;

            case 3: // Consultar Proximo Paciente
                int consultado = (tipo_fila == 1) ? consultar_max_heap(heap_fila, &paciente_aux) : consultar_frente_multi_fila(multi_fila, &paciente_aux);
                if (consultado) {
                    printf("\n[CONSULTA] Proximo paciente a ser chamado: %s (Prioridade: %d - %s). Estado: %s.\n", paciente_aux.nome, paciente_aux.prioridade, (paciente_aux.prioridade == 1 ? "Vermelho" : (paciente_aux.prioridade == 2 ? "Amarelo" : "Verde")), NOME_ESTADOS[paciente_aux.estado_atual]);
                } else {
                    printf("\n[INFO] A fila de pacientes esta vazia.\n");
                }
                break;

            case 4: // Exibir Fila de Espera
                if (tipo_fila == 1) {
                    exibir_heap(heap_fila);
                } else {
                    exibir_multi_fila(multi_fila);
                }
                break;

            case 5: // Ver possiveis proximos estados (Vizinhos)
                {
                    EstadoClinico estado_inicial = selecionar_estado();
                    exibir_vizinhos(fluxo_atendimento, estado_inicial);
                }
                break;

            case 6: // Ver todos os estados alcancaveis (Busca)
                {
                    EstadoClinico estado_inicial = selecionar_estado();
                    int tipo_busca;
                    printf("\n--- Selecao de Tipo de Busca ---\n");
                    printf("1. Busca em Largura (BFS)\n");
                    printf("2. Busca em Profundidade (DFS)\n");
                    printf("Escolha o tipo de busca (1 ou 2): ");
                    if (scanf("%d", &tipo_busca) != 1) {
                        printf("Entrada invalida. Usando BFS por padrao.\n");
                        tipo_busca = 1;
                    }
                    limpar_buffer();

                    if (tipo_busca == 2) {
                        busca_em_profundidade(fluxo_atendimento, estado_inicial);
                    } else {
                        busca_em_largura(fluxo_atendimento, estado_inicial);
                    }
                }
                break;

            case 7: // Exibir Relatorio Estatistico
                if (tipo_fila == 1) {
                    exibir_estatisticas(heap_fila);
                } else {
                    exibir_estatisticas_multi_fila(multi_fila);
                }
                break;

            case 8: // Simular Atendimento (Teste Automatico)
                {
                    int num_pacientes;
                    printf("Quantos pacientes deseja simular? ");
                    if (scanf("%d", &num_pacientes) != 1 || num_pacientes <= 0) {
                        printf("\n[AVISO] Numero invalido. Simulação cancelada.\n");
                        limpar_buffer();
                        break;
                    }
                    limpar_buffer();
                    simular_atendimento(heap_fila, multi_fila, num_pacientes, tipo_fila);
                }
                break;

            case 9:
                printf("\nEncerrando o Sistema de Triagem. Liberando memoria...\n");
                break;

            default:
                if (escolha != 0) {
                    printf("\n[AVISO] Opcao invalida. Por favor, escolha uma opcao de 1 a 9.\n");
                }
                break;
        }
    } while (escolha != 9);

    if (heap_fila) liberar_heap(heap_fila);
    if (multi_fila) liberar_multi_fila(multi_fila);
    liberar_grafo(fluxo_atendimento);
    return 0;
}
