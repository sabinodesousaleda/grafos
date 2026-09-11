#include <stdlib.h>
#include "busca_largura.h"

Fila *criar_fila(int capacidade)
{
    if (capacidade <= 0) return NULL;

    Fila *fila = malloc(sizeof(Fila));
    if (fila == NULL) return NULL;

    fila->dados = malloc(capacidade * sizeof(int));
    if (fila->dados == NULL) {
        free(fila);
        return NULL;
    }

    fila->capacidade = capacidade;
    fila->inicio = 0;
    fila->fim = 0;
    fila->tamanho = 0;

    return fila;
}

int enfileirar(Fila *fila, int valor)
{
    if (fila == NULL || fila->tamanho == fila->capacidade)
        return 0;

    fila->dados[fila->fim] = valor;
    fila->fim = (fila->fim + 1) % fila->capacidade;
    fila->tamanho++;

    return 1;
}

int desenfileirar(Fila *fila, int *valor)
{
    if (fila == NULL || valor == NULL || fila->tamanho == 0)
        return 0;

    *valor = fila->dados[fila->inicio];
    fila->inicio = (fila->inicio + 1) % fila->capacidade;
    fila->tamanho--;

    return 1;
}

int fila_vazia(Fila *fila)
{
    if (fila == NULL)
        return 1;

    return fila->tamanho == 0;
}

void liberar_fila(Fila *fila)
{
    if (fila == NULL)
        return;

    free(fila->dados);
    free(fila);
}

void bfs(GrafoLista *g, int origem, int *dist, int *pred)
{
    if (g == NULL || dist == NULL || pred == NULL)
        return;

    if (origem < 0 || origem >= g->n)
        return;

    for (int i = 0; i < g->n; i++) {
        dist[i] = -1;
        pred[i] = -1;
    }

    Fila *fila = criar_fila(g->n);
    if (fila == NULL)
        return;

    dist[origem] = 0;
    enfileirar(fila, origem);

    while (!fila_vazia(fila)) {
        int u;
        desenfileirar(fila, &u);

        for (No *p = g->adj[u]; p != NULL; p = p->prox) {
            int v = p->destino;

            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                pred[v] = u;
                enfileirar(fila, v);
            }
        }
    }

    liberar_fila(fila);
}