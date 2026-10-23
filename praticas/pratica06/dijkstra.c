#include <stdio.h>
#include <stdlib.h>
#include "dijkstra.h"
 
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
 
/* Aresta direcionada. Para grafo nao direcionado, chame duas vezes. */
void inserir_aresta_ponderada(GrafoListaPonderado *g, int origem, int destino, int peso) {
    AdjPonderado *novo = malloc(sizeof(AdjPonderado));
    if (novo == NULL) return;
    novo->destino = destino;
    novo->peso = peso;
    novo->prox = g->adj[origem];
    g->adj[origem] = novo;
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
 
/* ---------- Min-Heap ---------- */
 
MinHeap *criar_min_heap(int capacidade) {
    MinHeap *h = malloc(sizeof(MinHeap));
    if (h == NULL) return NULL;
    h->dados = malloc(capacidade * sizeof(NoHeap));
    h->posicao = malloc(capacidade * sizeof(int));
    h->capacidade = capacidade;
    h->tamanho = 0;
    if (h->dados == NULL || h->posicao == NULL) {
        free(h->dados);
        free(h->posicao);
        free(h);
        return NULL;
    }
    for (int i = 0; i < capacidade; i++) h->posicao[i] = -1;
    return h;
}
 
static void trocar(MinHeap *h, int i, int j) {
    NoHeap tmp = h->dados[i];
    h->dados[i] = h->dados[j];
    h->dados[j] = tmp;
    h->posicao[h->dados[i].vertice] = i;
    h->posicao[h->dados[j].vertice] = j;
}
 
static void subir(MinHeap *h, int i) {
    while (i > 0) {
        int pai = (i - 1) / 2;
        if (h->dados[i].distancia < h->dados[pai].distancia) {
            trocar(h, i, pai);
            i = pai;
        } else {
            break;
        }
    }
}
 
static void descer(MinHeap *h, int i) {
    for (;;) {
        int esq = 2 * i + 1;
        int dir = 2 * i + 2;
        int menor = i;
        if (esq < h->tamanho && h->dados[esq].distancia < h->dados[menor].distancia)
            menor = esq;
        if (dir < h->tamanho && h->dados[dir].distancia < h->dados[menor].distancia)
            menor = dir;
        if (menor == i) break;
        trocar(h, i, menor);
        i = menor;
    }
}
 
int heap_vazio(MinHeap *h) {
    return h->tamanho == 0;
}
 
void inserir_heap(MinHeap *h, int vertice, int distancia) {
    if (h->tamanho >= h->capacidade) return;
    int i = h->tamanho++;
    h->dados[i].vertice = vertice;
    h->dados[i].distancia = distancia;
    h->posicao[vertice] = i;
    subir(h, i);
}
 
NoHeap extrair_minimo(MinHeap *h) {
    NoHeap vazio = { -1, -1 };
    if (heap_vazio(h)) return vazio;
 
    NoHeap raiz = h->dados[0];
    trocar(h, 0, h->tamanho - 1);
    h->posicao[raiz.vertice] = -1;
    h->tamanho--;
    if (h->tamanho > 0) descer(h, 0);
    return raiz;
}
 
/* Diminui a distancia de um vertice que ja esta no heap: O(log V) */
void diminuir_chave(MinHeap *h, int vertice, int nova_distancia) {
    int i = h->posicao[vertice];
    if (i == -1) return;
    if (nova_distancia < h->dados[i].distancia) {
        h->dados[i].distancia = nova_distancia;
        subir(h, i);
    }
}
 
void liberar_min_heap(MinHeap *h) {
    if (h == NULL) return;
    free(h->dados);
    free(h->posicao);
    free(h);
}
 
/* ---------- Dijkstra ---------- */
 
void dijkstra(GrafoListaPonderado *g, int origem, int *dist, int *pred) {
    int n = g->num_vertices;
    MinHeap *h = criar_min_heap(n);
    if (h == NULL) return;
 
    for (int v = 0; v < n; v++) {
        dist[v] = INFINITO;
        pred[v] = -1;
    }
    dist[origem] = 0;
 
    for (int v = 0; v < n; v++) {
        inserir_heap(h, v, dist[v]);
    }
 
    while (!heap_vazio(h)) {
        NoHeap u = extrair_minimo(h);
        if (u.distancia == INFINITO) break; /* restantes sao inalcancaveis */
 
        for (AdjPonderado *a = g->adj[u.vertice]; a != NULL; a = a->prox) {
            int v = a->destino;
            if (h->posicao[v] == -1) continue; /* ja finalizado */
 
            long long nova = (long long)dist[u.vertice] + a->peso;
            if (nova < dist[v]) {
                dist[v] = (int)nova;
                pred[v] = u.vertice;
                diminuir_chave(h, v, dist[v]);
            }
        }
    }
 
    liberar_min_heap(h);
}
 
static void imprimir_rec(int v, int *pred) {
    if (pred[v] != -1) {
        imprimir_rec(pred[v], pred);
        printf(" -> ");
    }
    printf("%d", v);
}
 
void reconstruir_caminho(int origem, int destino, int *pred) {
    if (destino != origem && pred[destino] == -1) {
        printf("Sem caminho de %d ate %d\n", origem, destino);
        return;
    }
    imprimir_rec(destino, pred);
    printf("\n");
}