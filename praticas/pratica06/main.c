#include <stdio.h>
#include <stdlib.h>
#include "dijkstra.h"
 
/* Insere aresta nos dois sentidos (grafo nao direcionado) */
static void aresta(GrafoListaPonderado *g, int a, int b, int peso) {
    inserir_aresta_ponderada(g, a, b, peso);
    inserir_aresta_ponderada(g, b, a, peso);
}
 
int main(void) {
    int n = 7;
    GrafoListaPonderado *g = criar_grafo_ponderado(n);
 
    aresta(g, 0, 1, 7);
    aresta(g, 0, 2, 9);
    aresta(g, 0, 5, 14);
    aresta(g, 1, 2, 10);
    aresta(g, 1, 3, 15);
    aresta(g, 2, 3, 11);
    aresta(g, 2, 5, 2);
    aresta(g, 3, 4, 6);
    aresta(g, 4, 5, 9);
    /* vertice 6 fica isolado de proposito, para testar "sem caminho" */
 
    int *dist = malloc(n * sizeof(int));
    int *pred = malloc(n * sizeof(int));
    int origem = 0;
 
    dijkstra(g, origem, dist, pred);
 
    printf("Distancias a partir do vertice %d:\n", origem);
    for (int v = 0; v < n; v++) {
        if (dist[v] == INFINITO)
            printf("  ate %d: infinito\n", v);
        else
            printf("  ate %d: %d\n", v, dist[v]);
    }
 
    printf("\nCaminhos minimos:\n");
    for (int v = 0; v < n; v++) {
        printf("  %d -> %d: ", origem, v);
        reconstruir_caminho(origem, v, pred);
    }
 
    free(dist);
    free(pred);
    liberar_grafo_ponderado(g);
    return 0;
}