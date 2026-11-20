#include <limits.h>
#include <string.h>
#include "tps.h"

void grafo_para_tsp(const GrafoH *g, InstanciaTSP *t) {
    t->n = g->n;
    for (int i = 0; i < g->n; i++)
        for (int j = 0; j < g->n; j++)
            t->custo[i][j] = (i == j) ? 0 : (g->adj[i][j] ? 1 : 2);
}

/* ---------- Força bruta: origem fixa em 0, (n-1)! permutações ---------- */

typedef struct {
    const InstanciaTSP *t;
    int atual[MAX_V + 1];
    int usado[MAX_V];
    int melhor_custo;
    int melhor[MAX_V + 1];
} EstadoFB;

static void permuta(EstadoFB *e, int pos, int custo_parcial) {
    int n = e->t->n;
    if (pos == n) {
        int total = custo_parcial + e->t->custo[e->atual[n - 1]][0];
        if (total < e->melhor_custo) {
            e->melhor_custo = total;
            memcpy(e->melhor, e->atual, n * sizeof(int));
        }
        return;
    }
    for (int v = 1; v < n; v++) {
        if (!e->usado[v]) {
            e->usado[v] = 1;
            e->atual[pos] = v;
            permuta(e, pos + 1, custo_parcial + e->t->custo[e->atual[pos - 1]][v]);
            e->usado[v] = 0;
        }
    }
}

int tsp_forca_bruta(const InstanciaTSP *t, int rota[]) {
    EstadoFB e;
    e.t = t;
    memset(e.usado, 0, sizeof(e.usado));
    e.usado[0] = 1;
    e.atual[0] = 0;
    e.melhor_custo = INT_MAX;
    permuta(&e, 1, 0);
    memcpy(rota, e.melhor, t->n * sizeof(int));
    rota[t->n] = 0;
    return e.melhor_custo;
}

/* ---------- Heurística: vizinho mais próximo ---------- */

int tsp_vizinho_mais_proximo(const InstanciaTSP *t, int rota[]) {
    int n = t->n, usado[MAX_V] = {0}, custo = 0;
    rota[0] = 0;
    usado[0] = 1;
    for (int pos = 1; pos < n; pos++) {
        int ult = rota[pos - 1], prox = -1;
        for (int v = 0; v < n; v++)
            if (!usado[v] && (prox == -1 || t->custo[ult][v] < t->custo[ult][prox]))
                prox = v;
        rota[pos] = prox;
        usado[prox] = 1;
        custo += t->custo[ult][prox];
    }
    custo += t->custo[rota[n - 1]][0];
    rota[n] = 0;
    return custo;
}

/* ---------- Heurística: inserção do mais próximo ---------- */

int tsp_insercao(const InstanciaTSP *t, int rota[]) {
    int n = t->n, usado[MAX_V] = {0}, tam = 1;
    int ciclo[MAX_V + 1];          /* ciclo[0..tam-1], retorno implícito */
    ciclo[0] = 0;
    usado[0] = 1;

    while (tam < n) {
        /* 1) vértice fora do ciclo mais próximo de algum vértice dentro */
        int melhor_v = -1, melhor_d = INT_MAX;
        for (int v = 0; v < n; v++) {
            if (usado[v]) continue;
            for (int i = 0; i < tam; i++)
                if (t->custo[ciclo[i]][v] < melhor_d) {
                    melhor_d = t->custo[ciclo[i]][v];
                    melhor_v = v;
                }
        }
        /* 2) posição de inserção de menor aumento de custo */
        int melhor_pos = 1, menor_aumento = INT_MAX;
        for (int i = 0; i < tam; i++) {
            int a = ciclo[i], b = ciclo[(i + 1) % tam];
            int aumento = t->custo[a][melhor_v] + t->custo[melhor_v][b] - t->custo[a][b];
            if (aumento < menor_aumento) {
                menor_aumento = aumento;
                melhor_pos = i + 1;
            }
        }
        for (int i = tam; i > melhor_pos; i--) ciclo[i] = ciclo[i - 1];
        ciclo[melhor_pos] = melhor_v;
        usado[melhor_v] = 1;
        tam++;
    }

    int custo = 0;
    for (int i = 0; i < n; i++) {
        rota[i] = ciclo[i];
        custo += t->custo[ciclo[i]][ciclo[(i + 1) % n]];
    }
    rota[n] = ciclo[0];
    return custo;
}