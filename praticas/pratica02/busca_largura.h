#ifndef BUSCA_LARGURA_H
#define BUSCA_LARGURA_H

#include "../pratica01/grafo_lista.h"

/* Fila (FIFO) para BFS */
typedef struct {
    int *dados;
    int capacidade;
    int inicio;
    int fim;
    int tamanho;
} Fila;

/* Cria uma fila */
Fila *criar_fila(int capacidade);

/* Insere elemento na fila */
int enfileirar(Fila *fila, int valor);

/* Remove elemento da fila */
int desenfileirar(Fila *fila, int *valor);

/* Verifica se a fila está vazia */
int fila_vazia(Fila *fila);

/* Libera a fila */
void liberar_fila(Fila *fila);

/* Busca em largura */
void bfs(GrafoLista *g, int origem, int *dist, int *pred);

#endif