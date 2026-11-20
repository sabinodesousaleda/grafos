#include <string.h>
#include "hamiltoniano.h"

void iniciar_grafo_h(GrafoH *g, int n) {
    g->n = n;
    memset(g->adj, 0, sizeof(g->adj));
}

void inserir_aresta_h(GrafoH *g, int u, int v) {
    g->adj[u][v] = 1;
    g->adj[v][u] = 1;
}

/* Tenta estender o caminho parcial (posição pos). Se exigir_ciclo, o último
   vértice precisa ser adjacente ao primeiro. */
static int bt(const GrafoH *g, int caminho[], int usado[], int pos, int exigir_ciclo) {
    if (pos == g->n) {
        if (!exigir_ciclo) return 1;
        return g->adj[caminho[pos - 1]][caminho[0]];
    }
    for (int v = 0; v < g->n; v++) {
        if (!usado[v] && g->adj[caminho[pos - 1]][v]) {
            usado[v] = 1;
            caminho[pos] = v;
            if (bt(g, caminho, usado, pos + 1, exigir_ciclo)) return 1;
            usado[v] = 0;
        }
    }
    return 0;
}

int encontrar_caminho_hamiltoniano_bt(const GrafoH *g, int caminho[]) {
    int usado[MAX_V];
    for (int inicio = 0; inicio < g->n; inicio++) {
        memset(usado, 0, sizeof(usado));
        usado[inicio] = 1;
        caminho[0] = inicio;
        if (bt(g, caminho, usado, 1, 0)) return 1;
    }
    return 0;
}

int encontrar_ciclo_hamiltoniano_bt(const GrafoH *g, int ciclo[]) {
    int usado[MAX_V];
    memset(usado, 0, sizeof(usado));
    usado[0] = 1;      /* ciclo não depende do ponto de partida: fixa o 0 */
    ciclo[0] = 0;
    return bt(g, ciclo, usado, 1, 1);
}
