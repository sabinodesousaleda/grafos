#ifndef BELLMAN_FORD_H
#define BELLMAN_FORD_H
 
#include <limits.h>
 
#define INFINITO INT_MAX
 
/* ---------- Grafo ponderado (lista de adjacencia) ---------- */
 
typedef struct AdjPonderado {
    int destino;
    int peso;
    struct AdjPonderado *prox;
} AdjPonderado;
 
typedef struct {
    int num_vertices;
    AdjPonderado **adj;
} GrafoListaPonderado;
 
GrafoListaPonderado *criar_grafo_ponderado(int num_vertices);
void inserir_aresta_ponderada(GrafoListaPonderado *g, int origem, int destino, int peso);
void liberar_grafo_ponderado(GrafoListaPonderado *g);
 
/* ---------- Bellman-Ford ---------- */
 
/* Retorna 1 (sucesso) ou 0 (ciclo de custo negativo alcancavel da origem). */
int bellman_ford(GrafoListaPonderado *g, int origem, int *dist, int *pred);
 
/* Retrocede n vezes pelo pred[] para cair dentro do ciclo e imprime o ciclo.
   Retorna 1 se encontrou e imprimiu o ciclo, 0 caso contrario. */
int reconstruir_ciclo_negativo(GrafoListaPonderado *g, int *pred);
 
#endif