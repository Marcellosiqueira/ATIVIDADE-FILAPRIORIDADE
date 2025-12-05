#ifndef TIPOS_H
#define TIPOS_H

#include <time.h>

// Definição dos estados clínicos como vértices do grafo
typedef enum {
    AGUARDANDO_TRIAGEM,
    TRIADO_VERDE,
    TRIADO_AMARELO,
    TRIADO_VERMELHO,
    EM_ATENDIMENTO,
    EM_OBSERVACAO,
    LIBERADO,
    ENCAMINHADO_INTERNACAO
} EstadoClinico;

// Estrutura para o Paciente
typedef struct Paciente {
    char nome[50];
    // 1 (vermelho - emergencia), 2 (amarelo - urgencia), 3 (verde - nao urgente)
    int prioridade; 
    EstadoClinico estado_atual;
    time_t hora_chegada; // Para calculo de tempo de espera (Sugestao 2)
} Paciente;

#endif // TIPOS_H
