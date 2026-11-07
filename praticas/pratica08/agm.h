#ifndef AGM_H
#define AGM_H
 
#include <limits.h>
 
#define INFINITO INT_MAX
 
/* ---------- Estruturas obrigatorias ---------- */
 
typedef struct {
    int *pai, *rank;
    int n;
} DSU;
 
typedef struct {
    int u, v, peso;
} Aresta;
 
/* ---------- Grafo ponderado (lista de adjacencia), usado pelo Prim ---------- */
 
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
/* Insere a aresta nos dois sentidos (grafo nao direcionado). */
void inserir_aresta_ponderada(GrafoListaPonderado *g, int u, int v, int peso);
void liberar_grafo_ponderado(GrafoListaPonderado *g);
 
/* ---------- Union-Find ---------- */
 
DSU *criar_dsu(int n);
void liberar_dsu(DSU *d);
/* find com compressao de caminho. */
int find(DSU *d, int x);
/* uniao por rank. Retorna 1 se uniu, 0 se ja estavam no mesmo conjunto. */
int unir(DSU *d, int x, int y);
 
/* ---------- Arvore Geradora Minima ---------- */
 
/* Kruskal: O(E log E).
   Guarda em 'mst' as arestas escolhidas (precisa de espaco para n-1 arestas)
   e em '*qtd' quantas foram escolhidas. Retorna o peso total.
   Se o grafo for desconexo, devolve uma floresta geradora minima (qtd < n-1). */
int kruskal(const Aresta *arestas, int num_arestas, int n, Aresta *mst, int *qtd);
 
/* Prim com Min-Heap: O((V+E) log V).
   pai[v] = vertice que liga v na arvore (-1 para a raiz e para inalcancaveis).
   chave[v] = peso da aresta que liga v na arvore (INFINITO se inalcancavel).
   Retorna o peso total da arvore (apenas a componente da origem). */
int prim(GrafoListaPonderado *g, int origem, int *pai, int *chave);
 
#endif