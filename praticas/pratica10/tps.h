#ifndef TPS_H
#define TPS_H

#include "hamiltoniano.h"

/* Instância do TSP: matriz de custos completa e simétrica. */
typedef struct {
    int n;
    int custo[MAX_V][MAX_V];
} InstanciaTSP;

/* Redução: peso 1 para aresta existente, 2 para inexistente.
   Existe ciclo hamiltoniano no grafo <=> o TSP tem custo n. */
void grafo_para_tsp(const GrafoH *g, InstanciaTSP *t);

/* Todas retornam o custo do ciclo e preenchem rota[0..n], com rota[n] = origem. */
int tsp_forca_bruta(const InstanciaTSP *t, int rota[]);
int tsp_vizinho_mais_proximo(const InstanciaTSP *t, int rota[]);
int tsp_insercao(const InstanciaTSP *t, int rota[]);

#endif
