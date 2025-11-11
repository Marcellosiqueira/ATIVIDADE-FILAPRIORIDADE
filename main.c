#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fila_prioridade.h"

void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int menu() {
    int escolha;
    printf("\n--- Sistema de Triagem de Pronto-Socorro ---\n");
    printf("1. Adicionar Novo Paciente (Triagem)\n");
    printf("2. Chamar Proximo Paciente (Atendimento)\n");
    printf("3. Consultar Proximo Paciente\n");
    printf("4. Exibir Fila de Espera\n");
    printf("5. Sair\n");
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

    return novo_paciente;
}

int main() {
    FilaPrioridade *fila_pacientes = inicializar_fila();
    if (fila_pacientes == NULL) {
        fprintf(stderr, "Falha ao inicializar o sistema de triagem.\n");
        return 1;
    }

    int escolha;
    Paciente paciente_aux;

    do {
        escolha = menu();

        switch (escolha) {
            case 1:
                paciente_aux = obter_dados_paciente();
                if (enfileirar_prioridade(fila_pacientes, paciente_aux)) {
                    printf("\n[SUCESSO] Paciente %s adicionado a fila com prioridade %d.\n", paciente_aux.nome, paciente_aux.prioridade);
                } else {
                    printf("\n[ERRO] Falha ao adicionar paciente a fila.\n");
                }
                break;

            case 2:
                if (desenfileirar_prioridade(fila_pacientes, &paciente_aux)) {
                    printf("\n[ATENDIMENTO] Chamando paciente: %s (Prioridade: %d).\n", paciente_aux.nome, paciente_aux.prioridade);
                } else {
                    printf("\n[INFO] A fila de pacientes esta vazia. Sem pacientes para atender.\n");
                }
                break;

            case 3:
                if (consultar_frente(fila_pacientes, &paciente_aux)) {
                    printf("\n[CONSULTA] Proximo paciente a ser chamado: %s (Prioridade: %d).\n", paciente_aux.nome, paciente_aux.prioridade);
                } else {
                    printf("\n[INFO] A fila de pacientes esta vazia.\n");
                }
                break;

            case 4:
                exibir_fila(fila_pacientes);
                break;

            case 5:
                printf("\nEncerrando o Sistema de Triagem. Liberando memoria...\n");
                break;

            default:
                if (escolha != 0) {
                    printf("\n[AVISO] Opcao invalida. Por favor, escolha uma opcao de 1 a 5.\n");
                }
                break;
        }
    } while (escolha != 5);

    liberar_fila(fila_pacientes);
    return 0;
}
