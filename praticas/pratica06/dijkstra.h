#ifndef DIJKSTRA_H
#define DIJKSTRA_H
 
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
 
/* ---------- Min-Heap ---------- */
 
typedef struct {
    int vertice;
    int distancia;
} NoHeap;
 
typedef struct {
    NoHeap *dados;
    int capacidade, tamanho;
    int *posicao; /* mapeia vertice -> indice no heap (-1 se fora do heap) */
} MinHeap;
 
MinHeap *criar_min_heap(int capacidade);
void inserir_heap(MinHeap *h, int vertice, int distancia);
NoHeap extrair_minimo(MinHeap *h);
void diminuir_chave(MinHeap *h, int vertice, int nova_distancia);
int heap_vazio(MinHeap *h);
void liberar_min_heap(MinHeap *h);
 
/* ---------- Dijkstra ---------- */
 
void dijkstra(GrafoListaPonderado *g, int origem, int *dist, int *pred);
void reconstruir_caminho(int origem, int destino, int *pred);
 
#endif
 