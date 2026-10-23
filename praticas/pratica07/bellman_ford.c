#include <stdio.h>
#include <stdlib.h>
#include "bellman_ford.h"
 
/* ---------- Grafo ponderado ---------- */
 
GrafoListaPonderado *criar_grafo_ponderado(int num_vertices) {
    GrafoListaPonderado *g = malloc(sizeof(GrafoListaPonderado));
    if (g == NULL) return NULL;
    g->num_vertices = num_vertices;
    g->adj = calloc(num_vertices, sizeof(AdjPonderado *));
    if (g->adj == NULL) {
        free(g);
        return NULL;
    }
    return g;
}
 
/* Aresta direcionada: origem -> destino. */
void inserir_aresta_ponderada(GrafoListaPonderado *g, int origem, int destino, int peso) {
    AdjPonderado *novo = malloc(sizeof(AdjPonderado));
    if (novo == NULL) return;
    novo->destino = destino;
    novo->peso = peso;
    novo->prox = g->adj[origem];
    g->adj[origem] = novo;
}
 
void liberar_grafo_ponderado(GrafoListaPonderado *g) {
    if (g == NULL) return;
    for (int i = 0; i < g->num_vertices; i++) {
        AdjPonderado *atual = g->adj[i];
        while (atual != NULL) {
            AdjPonderado *prox = atual->prox;
            free(atual);
            atual = prox;
        }
    }
    free(g->adj);
    free(g);
}
 
/* ---------- Bellman-Ford ---------- */
 
int bellman_ford(GrafoListaPonderado *g, int origem, int *dist, int *pred) {
    int n = g->num_vertices;
 
    for (int v = 0; v < n; v++) {
        dist[v] = INFINITO;
        pred[v] = -1;
    }
    dist[origem] = 0;
 
    /* Sem ciclo negativo, n-1 iteracoes bastam. Se a n-esima iteracao
       ainda relaxar alguma aresta, existe ciclo de custo negativo. */
    for (int i = 1; i <= n; i++) {
        int relaxou = 0;
 
        for (int u = 0; u < n; u++) {
            if (dist[u] == INFINITO) continue;
 
            for (AdjPonderado *a = g->adj[u]; a != NULL; a = a->prox) {
                long long nova = (long long)dist[u] + a->peso;
                if (nova < dist[a->destino]) {
                    dist[a->destino] = (int)nova;
                    pred[a->destino] = u;
                    relaxou = 1;
                }
            }
        }
 
        if (!relaxou) return 1; /* estabilizou: sem ciclo negativo */
    }
 
    return 0; /* todas as n iteracoes relaxaram: ciclo negativo */
}
 
int reconstruir_ciclo_negativo(GrafoListaPonderado *g, int *pred) {
    int n = g->num_vertices;
    int *ciclo = malloc(n * sizeof(int));
    if (ciclo == NULL) return 0;
 
    for (int s = 0; s < n; s++) {
        /* Retrocede n vezes: se nunca chegar em -1, cai dentro de um ciclo. */
        int x = s;
        for (int i = 0; i < n && x != -1; i++) {
            x = pred[x];
        }
        if (x == -1) continue;
 
        /* x esta no ciclo: segue o pred ate voltar nele. */
        int k = 0;
        int y = x;
        do {
            ciclo[k++] = y;
            y = pred[y];
        } while (y != x);
 
        /* O pred anda "para tras", entao imprime na ordem inversa. */
        printf("Ciclo negativo: %d", x);
        for (int j = k - 1; j >= 1; j--) {
            printf(" -> %d", ciclo[j]);
        }
        printf(" -> %d\n", x);
 
        free(ciclo);
        return 1;
    }
 
    free(ciclo);
    return 0;
}
 