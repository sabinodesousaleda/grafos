#include <stdlib.h>
#include "busca_profundidade.h"

void dfs_recursiva(
    GrafoLista *g,
    int u,
    int *visitado,
    int *tempo,
    int *entrada,
    int *saida
)
{
    if (g == NULL || visitado == NULL || tempo == NULL ||
        entrada == NULL || saida == NULL) {
        return;
    }

    visitado[u] = 1;

    (*tempo)++;
    entrada[u] = *tempo;

    for (No *p = g->adj[u]; p != NULL; p = p->prox) {
        int v = p->destino;

        if (!visitado[v]) {
            dfs_recursiva(
                g,
                v,
                visitado,
                tempo,
                entrada,
                saida
            );
        }
    }

    (*tempo)++;
    saida[u] = *tempo;
}

int contar_componentes(GrafoLista *g)
{
    if (g == NULL) {
        return 0;
    }

    int *visitado = calloc(g->n, sizeof(int));
    int *entrada = malloc(g->n * sizeof(int));
    int *saida = malloc(g->n * sizeof(int));

    if (visitado == NULL || entrada == NULL || saida == NULL) {
        free(visitado);
        free(entrada);
        free(saida);
        return 0;
    }

    int tempo = 0;
    int componentes = 0;

    for (int i = 0; i < g->n; i++) {
        entrada[i] = 0;
        saida[i] = 0;
    }

    for (int i = 0; i < g->n; i++) {
        if (!visitado[i]) {
            componentes++;

            dfs_recursiva(
                g,
                i,
                visitado,
                &tempo,
                entrada,
                saida
            );
        }
    }

    free(visitado);
    free(entrada);
    free(saida);

    return componentes;
}

static int dfs_ciclo(
    GrafoLista *g,
    int u,
    int pai,
    int *visitado
)
{
    visitado[u] = 1;

    for (No *p = g->adj[u]; p != NULL; p = p->prox) {
        int v = p->destino;

        if (!visitado[v]) {
            if (dfs_ciclo(g, v, u, visitado)) {
                return 1;
            }
        } else if (v != pai) {
            return 1;
        }
    }

    return 0;
}

int tem_ciclo(GrafoLista *g)
{
    if (g == NULL) {
        return 0;
    }

    int *visitado = calloc(g->n, sizeof(int));

    if (visitado == NULL) {
        return 0;
    }

    for (int i = 0; i < g->n; i++) {
        if (!visitado[i]) {
            if (dfs_ciclo(g, i, -1, visitado)) {
                free(visitado);
                return 1;
            }
        }
    }

    free(visitado);

    return 0;
}

static int bfs_bipartido(
    GrafoLista *g,
    int inicio,
    int *cor
)
{
    int *fila = malloc(g->n * sizeof(int));

    if (fila == NULL) {
        return 0;
    }

    int inicio_f = 0;
    int fim_f = 0;

    cor[inicio] = 1;
    fila[fim_f++] = inicio;

    while (inicio_f < fim_f) {
        int u = fila[inicio_f++];

        for (No *p = g->adj[u]; p != NULL; p = p->prox) {
            int v = p->destino;

            if (cor[v] == 0) {
                cor[v] = -cor[u];
                fila[fim_f++] = v;
            } else if (cor[v] == cor[u]) {
                free(fila);
                return 0;
            }
        }
    }

    free(fila);

    return 1;
}

int eh_bipartido(GrafoLista *g)
{
    if (g == NULL) {
        return 0;
    }

    int *cor = calloc(g->n, sizeof(int));

    if (cor == NULL) {
        return 0;
    }

    for (int i = 0; i < g->n; i++) {
        if (cor[i] == 0) {
            if (!bfs_bipartido(g, i, cor)) {
                free(cor);
                return 0;
            }
        }
    }

    free(cor);

    return 1;
}