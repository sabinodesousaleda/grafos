#include <stdio.h>
#include <stdlib.h>
#include "agm.h"
 
/* ---------- Grafo ponderado ---------- */
 
GrafoListaPonderado *criar_grafo_ponderado(int num_vertices) {
    GrafoListaPonderado *g = malloc(sizeof(GrafoListaPonderado));
    if (g == NULL) return NULL;
    g->num_vertices = num_vertices;
    g->adj = calloc(num_vertices, sizeof(AdjPonderado *));
    if (g->adj == NULL) {
        free(g);
        return NULL;
    }
    return g;
}
 
static void inserir_direcionada(GrafoListaPonderado *g, int origem, int destino, int peso) {
    AdjPonderado *novo = malloc(sizeof(AdjPonderado));
    if (novo == NULL) return;
    novo->destino = destino;
    novo->peso = peso;
    novo->prox = g->adj[origem];
    g->adj[origem] = novo;
}
 
void inserir_aresta_ponderada(GrafoListaPonderado *g, int u, int v, int peso) {
    inserir_direcionada(g, u, v, peso);
    inserir_direcionada(g, v, u, peso);
}
 
void liberar_grafo_ponderado(GrafoListaPonderado *g) {
    if (g == NULL) return;
    for (int i = 0; i < g->num_vertices; i++) {
        AdjPonderado *atual = g->adj[i];
        while (atual != NULL) {
            AdjPonderado *prox = atual->prox;
            free(atual);
            atual = prox;
        }
    }
    free(g->adj);
    free(g);
}
 
/* ---------- Union-Find ---------- */
 
DSU *criar_dsu(int n) {
    DSU *d = malloc(sizeof(DSU));
    if (d == NULL) return NULL;
    d->n = n;
    d->pai = malloc(n * sizeof(int));
    d->rank = calloc(n, sizeof(int));
    if (d->pai == NULL || d->rank == NULL) {
        free(d->pai);
        free(d->rank);
        free(d);
        return NULL;
    }
    for (int i = 0; i < n; i++) d->pai[i] = i;
    return d;
}
 
void liberar_dsu(DSU *d) {
    if (d == NULL) return;
    free(d->pai);
    free(d->rank);
    free(d);
}
 
int find(DSU *d, int x) {
    /* Primeiro acha a raiz, depois aponta todo o caminho direto para ela. */
    int raiz = x;
    while (d->pai[raiz] != raiz) raiz = d->pai[raiz];
 
    while (d->pai[x] != raiz) {
        int prox = d->pai[x];
        d->pai[x] = raiz;
        x = prox;
    }
    return raiz;
}
 
int unir(DSU *d, int x, int y) {
    int rx = find(d, x);
    int ry = find(d, y);
    if (rx == ry) return 0;
 
    /* A arvore de menor rank vira filha da de maior rank. */
    if (d->rank[rx] < d->rank[ry]) {
        d->pai[rx] = ry;
    } else if (d->rank[rx] > d->rank[ry]) {
        d->pai[ry] = rx;
    } else {
        d->pai[ry] = rx;
        d->rank[rx]++;
    }
    return 1;
}
 
/* ---------- Kruskal ---------- */
 
static int comparar_arestas(const void *a, const void *b) {
    const Aresta *x = a;
    const Aresta *y = b;
    if (x->peso < y->peso) return -1;
    if (x->peso > y->peso) return 1;
    return 0;
}
 
int kruskal(const Aresta *arestas, int num_arestas, int n, Aresta *mst, int *qtd) {
    *qtd = 0;
    if (n <= 0) return 0;
 
    /* Ordena uma copia para nao alterar o vetor original. */
    Aresta *ordenadas = malloc((num_arestas > 0 ? num_arestas : 1) * sizeof(Aresta));
    DSU *d = criar_dsu(n);
    if (ordenadas == NULL || d == NULL) {
        free(ordenadas);
        liberar_dsu(d);
        return 0;
    }
    for (int i = 0; i < num_arestas; i++) ordenadas[i] = arestas[i];
    qsort(ordenadas, num_arestas, sizeof(Aresta), comparar_arestas);
 
    int total = 0;
    for (int i = 0; i < num_arestas && *qtd < n - 1; i++) {
        if (unir(d, ordenadas[i].u, ordenadas[i].v)) {
            mst[(*qtd)++] = ordenadas[i];
            total += ordenadas[i].peso;
        }
    }
 
    free(ordenadas);
    liberar_dsu(d);
    return total;
}
 
/* ---------- Min-Heap (usado pelo Prim) ---------- */
 
typedef struct {
    int vertice;
    int chave;
} NoHeap;
 
typedef struct {
    NoHeap *dados;
    int tamanho;
    int *posicao; /* vertice -> indice no heap (-1 se fora do heap) */
} MinHeap;
 
static void heap_trocar(MinHeap *h, int i, int j) {
    NoHeap tmp = h->dados[i];
    h->dados[i] = h->dados[j];
    h->dados[j] = tmp;
    h->posicao[h->dados[i].vertice] = i;
    h->posicao[h->dados[j].vertice] = j;
}
 
static void heap_subir(MinHeap *h, int i) {
    while (i > 0) {
        int pai = (i - 1) / 2;
        if (h->dados[i].chave >= h->dados[pai].chave) break;
        heap_trocar(h, i, pai);
        i = pai;
    }
}
 
static void heap_descer(MinHeap *h, int i) {
    for (;;) {
        int esq = 2 * i + 1;
        int dir = 2 * i + 2;
        int menor = i;
        if (esq < h->tamanho && h->dados[esq].chave < h->dados[menor].chave) menor = esq;
        if (dir < h->tamanho && h->dados[dir].chave < h->dados[menor].chave) menor = dir;
        if (menor == i) break;
        heap_trocar(h, i, menor);
        i = menor;
    }
}
 
static NoHeap heap_extrair_minimo(MinHeap *h) {
    NoHeap raiz = h->dados[0];
    heap_trocar(h, 0, h->tamanho - 1);
    h->posicao[raiz.vertice] = -1;
    h->tamanho--;
    if (h->tamanho > 0) heap_descer(h, 0);
    return raiz;
}
 
static void heap_diminuir_chave(MinHeap *h, int vertice, int nova_chave) {
    int i = h->posicao[vertice];
    if (i == -1 || nova_chave >= h->dados[i].chave) return;
    h->dados[i].chave = nova_chave;
    heap_subir(h, i);
}
 
/* ---------- Prim ---------- */
 
int prim(GrafoListaPonderado *g, int origem, int *pai, int *chave) {
    int n = g->num_vertices;
 
    MinHeap h;
    h.dados = malloc(n * sizeof(NoHeap));
    h.posicao = malloc(n * sizeof(int));
    if (h.dados == NULL || h.posicao == NULL) {
        free(h.dados);
        free(h.posicao);
        return 0;
    }
    h.tamanho = 0;
 
    for (int v = 0; v < n; v++) {
        pai[v] = -1;
        chave[v] = (v == origem) ? 0 : INFINITO;
        h.dados[v].vertice = v;
        h.dados[v].chave = chave[v];
        h.posicao[v] = v;
    }
    h.tamanho = n;
    heap_trocar(&h, 0, origem); /* coloca a origem (chave 0) na raiz */
 
    int total = 0;
    while (h.tamanho > 0) {
        NoHeap u = heap_extrair_minimo(&h);
        if (u.chave == INFINITO) break; /* o resto nao e alcancavel */
        total += u.chave;
 
        for (AdjPonderado *a = g->adj[u.vertice]; a != NULL; a = a->prox) {
            int v = a->destino;
            if (h.posicao[v] != -1 && a->peso < chave[v]) {
                chave[v] = a->peso;
                pai[v] = u.vertice;
                heap_diminuir_chave(&h, v, a->peso);
            }
        }
    }
 
    free(h.dados);
    free(h.posicao);
    return total;
}