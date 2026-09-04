#include <stdlib.h>
#include "busca_largura.h"

int *busca_em_largura(GrafoLista *g, int inicio) {
    if (!g || inicio < 0 || inicio >= g->n) return NULL;

    int *dist = malloc(g->n * sizeof(int));
    int *fila = malloc(g->n * sizeof(int));
    if (!dist || !fila) return NULL;

    for (int i = 0; i < g->n; i++) dist[i] = -1;

    int inicio_f = 0, fim_f = 0;

    dist[inicio] = 0;
    fila[fim_f++] = inicio;

    while (inicio_f < fim_f) {
        int u = fila[inicio_f++];
        for (No *p = g->adj[u]; p != NULL; p = p->prox) {
            int v = p->destino;
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                fila[fim_f++] = v;
            }
        }
    }

    free(fila);
    return dist;
}