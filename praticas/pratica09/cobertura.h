#ifndef COBERTURA_H
#define COBERTURA_H
 
#define MAX_V 32
 
/* Grafo nao direcionado com matriz de adjacencia (suficiente para n pequeno). */
typedef struct {
    int n;
    int adj[MAX_V][MAX_V];
} GrafoCobertura;
 
void grafo_iniciar(GrafoCobertura *g, int n);
void grafo_inserir_aresta(GrafoCobertura *g, int u, int v);
int grafo_num_arestas(const GrafoCobertura *g);
 
/* 'cobertura' e um vetor de n posicoes: cobertura[v] = 1 se v esta na cobertura. */
 
/* Retorna 1 se todas as arestas tem pelo menos uma ponta na cobertura, 0 caso contrario. */
int eh_cobertura(const GrafoCobertura *g, const int *cobertura);
 
/* Heuristica gulosa: escolhe sempre o vertice de maior grau restante.
   Preenche 'cobertura' e retorna o tamanho. Nao garante o otimo. */
int cobertura_gulosa(const GrafoCobertura *g, int *cobertura);
 
/* Forca bruta com backtracking e poda por limite.
   Preenche 'cobertura' com uma cobertura MINIMA e retorna o tamanho dela. */
int cobertura_minima_bt(const GrafoCobertura *g, int *cobertura);
 
#endif