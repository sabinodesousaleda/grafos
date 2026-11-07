#include <stdio.h>
#include <stdlib.h>
#include "agm.h"
 
#define N 6
 
int main(void) {
    /* Grafo nao direcionado, conexo, com 6 vertices e 9 arestas. */
    Aresta arestas[] = {
        {0, 1, 4}, {0, 2, 4}, {1, 2, 2},
        {2, 3, 3}, {2, 5, 2}, {2, 4, 4},
        {3, 4, 3}, {5, 4, 3}, {0, 5, 7}
    };
    int num_arestas = sizeof(arestas) / sizeof(arestas[0]);
 
    /* ---------- Kruskal ---------- */
    Aresta mst[N];
    int qtd;
    int peso_kruskal = kruskal(arestas, num_arestas, N, mst, &qtd);
 
    printf("=== Kruskal ===\n");
    for (int i = 0; i < qtd; i++) {
        printf("  %d - %d (peso %d)\n", mst[i].u, mst[i].v, mst[i].peso);
    }
    printf("Peso total: %d\n", peso_kruskal);
 
    /* ---------- Prim ---------- */
    GrafoListaPonderado *g = criar_grafo_ponderado(N);
    for (int i = 0; i < num_arestas; i++) {
        inserir_aresta_ponderada(g, arestas[i].u, arestas[i].v, arestas[i].peso);
    }
 
    int pai[N], chave[N];
    int peso_prim = prim(g, 0, pai, chave);
 
    printf("\n=== Prim (origem 0) ===\n");
    for (int v = 0; v < N; v++) {
        if (pai[v] != -1)
            printf("  %d - %d (peso %d)\n", pai[v], v, chave[v]);
    }
    printf("Peso total: %d\n", peso_prim);
 
    printf("\n%s\n", peso_kruskal == peso_prim
           ? "Kruskal e Prim acharam o mesmo peso total."
           : "ERRO: pesos diferentes!");
 
    liberar_grafo_ponderado(g);
    return 0;
}