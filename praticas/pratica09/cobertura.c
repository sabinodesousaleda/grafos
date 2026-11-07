#include <stdio.h>
#include <string.h>
#include "cobertura.h"
 
/* ---------- Grafo ---------- */
 
void grafo_iniciar(GrafoCobertura *g, int n) {
    g->n = n;
    memset(g->adj, 0, sizeof(g->adj));
}
 
void grafo_inserir_aresta(GrafoCobertura *g, int u, int v) {
    if (u == v) return;
    g->adj[u][v] = 1;
    g->adj[v][u] = 1;
}
 
int grafo_num_arestas(const GrafoCobertura *g) {
    int m = 0;
    for (int u = 0; u < g->n; u++)
        for (int v = u + 1; v < g->n; v++)
            if (g->adj[u][v]) m++;
    return m;
}
 
/* ---------- Verificacao ---------- */
 
int eh_cobertura(const GrafoCobertura *g, const int *cobertura) {
    for (int u = 0; u < g->n; u++) {
        for (int v = u + 1; v < g->n; v++) {
            if (g->adj[u][v] && !cobertura[u] && !cobertura[v]) {
                return 0; /* aresta (u, v) descoberta */
            }
        }
    }
    return 1;
}
 
/* ---------- Heuristica gulosa (maior grau primeiro) ---------- */
 
int cobertura_gulosa(const GrafoCobertura *g, int *cobertura) {
    int n = g->n;
    int grau[MAX_V]; /* grau[v] = arestas ainda NAO cobertas que tocam v */
    int tamanho = 0;
 
    for (int v = 0; v < n; v++) {
        cobertura[v] = 0;
        grau[v] = 0;
        for (int u = 0; u < n; u++) grau[v] += g->adj[v][u];
    }
 
    for (;;) {
        int melhor = -1;
        for (int v = 0; v < n; v++) {
            if (!cobertura[v] && grau[v] > 0 && (melhor == -1 || grau[v] > grau[melhor]))
                melhor = v;
        }
        if (melhor == -1) break; /* nenhuma aresta restante */
 
        cobertura[melhor] = 1;
        tamanho++;
 
        /* As arestas de 'melhor' agora estao cobertas: baixa o grau dos vizinhos. */
        for (int u = 0; u < n; u++) {
            if (g->adj[melhor][u] && !cobertura[u]) grau[u]--;
        }
        grau[melhor] = 0;
    }
 
    return tamanho;
}
 
/* ---------- Backtracking com poda ---------- */
 
typedef struct {
    const GrafoCobertura *g;
    int atual[MAX_V];   /* cobertura sendo construida */
    int tam_atual;
    int melhor[MAX_V];  /* melhor cobertura encontrada */
    int tam_melhor;
} EstadoBT;
 
static void backtrack(EstadoBT *s) {
    const GrafoCobertura *g = s->g;
    int n = g->n;
 
    /* Procura a primeira aresta descoberta e calcula o limite inferior. */
    int eu = -1, ev = -1;
    int arestas_restantes = 0;
    int grau[MAX_V] = {0};
 
    for (int u = 0; u < n; u++) {
        for (int v = u + 1; v < n; v++) {
            if (g->adj[u][v] && !s->atual[u] && !s->atual[v]) {
                if (eu == -1) {
                    eu = u;
                    ev = v;
                }
                arestas_restantes++;
                grau[u]++;
                grau[v]++;
            }
        }
    }
 
    if (eu == -1) { /* todas cobertas: solucao completa */
        if (s->tam_atual < s->tam_melhor) {
            s->tam_melhor = s->tam_atual;
            memcpy(s->melhor, s->atual, sizeof(s->atual));
        }
        return;
    }
 
    /* Poda por limite: cada vertice novo cobre no maximo 'grau_max' arestas,
       entao faltam pelo menos ceil(arestas_restantes / grau_max) vertices. */
    int grau_max = 0;
    for (int v = 0; v < n; v++)
        if (grau[v] > grau_max) grau_max = grau[v];
    int faltam = (arestas_restantes + grau_max - 1) / grau_max;
 
    if (s->tam_atual + faltam >= s->tam_melhor) return;
 
    /* A aresta (eu, ev) precisa de uma das duas pontas na cobertura. */
    s->atual[eu] = 1;
    s->tam_atual++;
    backtrack(s);
    s->atual[eu] = 0;
    s->tam_atual--;
 
    s->atual[ev] = 1;
    s->tam_atual++;
    backtrack(s);
    s->atual[ev] = 0;
    s->tam_atual--;
}
 
int cobertura_minima_bt(const GrafoCobertura *g, int *cobertura) {
    EstadoBT s;
    s.g = g;
    s.tam_atual = 0;
    memset(s.atual, 0, sizeof(s.atual));
 
    /* Comeca com a solucao gulosa como limite superior: so procuramos algo melhor. */
    s.tam_melhor = cobertura_gulosa(g, s.melhor);
 
    backtrack(&s);
 
    for (int v = 0; v < g->n; v++) cobertura[v] = s.melhor[v];
    return s.tam_melhor;
}