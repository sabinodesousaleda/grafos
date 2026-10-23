#include <stdio.h>
#include <stdlib.h>
#include "bellman_ford.h"
 
static void imprimir_distancias(int n, int *dist, int origem) {
    printf("Distancias a partir do vertice %d:\n", origem);
    for (int v = 0; v < n; v++) {
        if (dist[v] == INFINITO)
            printf("  ate %d: infinito\n", v);
        else
            printf("  ate %d: %d\n", v, dist[v]);
    }
}
 
static void executar(GrafoListaPonderado *g, int origem) {
    int n = g->num_vertices;
    int *dist = malloc(n * sizeof(int));
    int *pred = malloc(n * sizeof(int));
 
    if (bellman_ford(g, origem, dist, pred)) {
        printf("Sem ciclo negativo.\n");
        imprimir_distancias(n, dist, origem);
    } else {
        printf("Ciclo de custo negativo detectado!\n");
        reconstruir_ciclo_negativo(g, pred);
    }
 
    free(dist);
    free(pred);
}
 
int main(void) {
    /* Teste 1: pesos negativos, mas sem ciclo negativo.
       O vertice 5 fica isolado de proposito. */
    printf("=== Teste 1: grafo com arestas negativas, sem ciclo negativo ===\n");
    GrafoListaPonderado *g1 = criar_grafo_ponderado(6);
    inserir_aresta_ponderada(g1, 0, 1, 6);
    inserir_aresta_ponderada(g1, 0, 2, 7);
    inserir_aresta_ponderada(g1, 1, 2, 8);
    inserir_aresta_ponderada(g1, 1, 3, 5);
    inserir_aresta_ponderada(g1, 1, 4, -4);
    inserir_aresta_ponderada(g1, 2, 3, -3);
    inserir_aresta_ponderada(g1, 2, 4, 9);
    inserir_aresta_ponderada(g1, 3, 1, -2);
    inserir_aresta_ponderada(g1, 4, 3, 7);
    inserir_aresta_ponderada(g1, 4, 0, 2);
    executar(g1, 0);
    liberar_grafo_ponderado(g1);
 
    /* Teste 2: ciclo 1 -> 2 -> 3 -> 1 com custo total -3. */
    printf("\n=== Teste 2: grafo com ciclo de custo negativo ===\n");
    GrafoListaPonderado *g2 = criar_grafo_ponderado(5);
    inserir_aresta_ponderada(g2, 0, 1, 1);
    inserir_aresta_ponderada(g2, 1, 2, -1);
    inserir_aresta_ponderada(g2, 2, 3, -1);
    inserir_aresta_ponderada(g2, 3, 1, -1);
    inserir_aresta_ponderada(g2, 3, 4, 2);
    executar(g2, 0);
    liberar_grafo_ponderado(g2);
 
    return 0;
}