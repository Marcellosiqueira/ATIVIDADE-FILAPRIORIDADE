# Projeto de Estrutura de Dados: Fila de Prioridades

## 👨‍💻 Integrantes
*   [Nome do Integrante 1]
*   [Nome do Integrante 2]
*   [Nome do Integrante 3]
*   [Nome do Integrante 4]

## 📚 Estrutura de Dados Implementada
**Fila com Prioridades (Priority Queue)**

## 🏥 Aplicacao Pratica
**Sistema de Triagem de Pacientes em Pronto-Socorro**

A aplicacao simula o processo de triagem de pacientes em um pronto-socorro, onde o atendimento e baseada na prioridadede** definida no momento da entrada (triagem), e nao apenas na ordem de chegada.

### Funcionamento da Prioridade
A prioridade e definida por um sistema de cores, onde o menor numero representa a maior prioridade:
*   **1 (Vermelho):** Emergencia (Maior Prioridade)
*   **2 (Amarelo):** Urgencia
*   **3 (Verde):** Nao Urgente (Menor Prioridade)

A Fila de Prioridades garante que o paciente com a prioridade mais alta seja sempre o proximo a ser atendido. Em caso de prioridades iguais, a ordem de chegada (FIFO) e mantida.

## 🛠️ Instrucoes de Compilacao e Execucao

### Pre-requisitos
E necessario ter o compilador **GCC** instalado no sistema.

### Compilacao
Navegue ate o diretorio do projeto e utilize o seguinte comando para compilar a aplicacao:

```bash
gcc -o triagem main.c fila_prioridade.c
```

### Execucao
Apos a compilacao, execute o programa com o seguinte comando:

```bash
./triagem
```

O programa apresentara um menu interativo para adicionar pacientes, chamar o proximo para atendimento, consultar o proximo e exibir a fila de espera.

## 📂 Estrutura do Codigo
O projeto segue a modularizacao exigida, sendo dividido em tres arquivos principais:

1.  **`main.c`**: Contem a logica principal da aplicacao (Sistema de Triagem) e a interface de usuario (menu).
2.  **`fila_prioridade.h`**: CContem a definicao das estruturas (Paciente, No, FilaPrioridade) e os prototipos das funcoes da Fila de Prioridades.ades.
3.  **`fila_prioridade.c`**: Contem a implementacao das funcoes essenciais da Fila de Prioridades, utilizando alocacao dinamica de memoria.
    *   `inicializar_fila()`
    *   `fila_vazia()`
    *   `enfileirar_prioridade()` (Funcao de Insercao)
    *   `desenfileirar_prioridade()` (Funcao de Remocao)
    *   `consultar_frente()` (Funcao de Consulta)
    *   `exibir_fila()`
    *   `liberar_fila()` (Liberacao de Memoria)
