#ifndef HAMILTONIANO_H
#define HAMILTONIANO_H

#define MAX_V 16

/* Grafo não direcionado por matriz de adjacência (autocontido). */
typedef struct {
    int n;
    int adj[MAX_V][MAX_V];
} GrafoH;

void iniciar_grafo_h(GrafoH *g, int n);
void inserir_aresta_h(GrafoH *g, int u, int v);

/* Backtracking com permutações.
   Retornam 1 se achou (e preenche o vetor), 0 caso contrário.
   caminho: n posições. ciclo: n posições (o retorno ao início é implícito). */
int encontrar_caminho_hamiltoniano_bt(const GrafoH *g, int caminho[]);
int encontrar_ciclo_hamiltoniano_bt(const GrafoH *g, int ciclo[]);

#endif
