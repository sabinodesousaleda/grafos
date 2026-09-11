#ifndef BUSCA_PROFUNDIDADE_H
#define BUSCA_PROFUNDIDADE_H

#include "../pratica01/grafo_lista.h"

/* Pilha (LIFO) para DFS iterativa */
typedef struct {
    int *dados;
    int topo;
    int capacidade;
} Pilha;

/* DFS recursiva com tempos de entrada e saída */
void dfs_recursiva(
    GrafoLista *g,
    int u,
    int *visitado,
    int *tempo,
    int *entrada,
    int *saida
);

/* Conta componentes conexos */
int contar_componentes(GrafoLista *g);

/* Verifica se o grafo possui ciclo */
int tem_ciclo(GrafoLista *g);

/* Verifica se o grafo é bipartido */
int eh_bipartido(GrafoLista *g);

#endif