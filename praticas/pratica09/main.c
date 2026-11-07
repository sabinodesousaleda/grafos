#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "cobertura.h"
 
typedef int (*Algoritmo)(const GrafoCobertura *, int *);
 
static void imprimir_cobertura(const GrafoCobertura *g, const int *cobertura) {
    printf("{");
    int primeiro = 1;
    for (int v = 0; v < g->n; v++) {
        if (cobertura[v]) {
            printf("%s%d", primeiro ? "" : ", ", v);
            primeiro = 0;
        }
    }
    printf("}");
}
 
static void gerar_grafo_aleatorio(GrafoCobertura *g, int n, int percentual) {
    grafo_iniciar(g, n);
    for (int u = 0; u < n; u++)
        for (int v = u + 1; v < n; v++)
            if (rand() % 100 < percentual) grafo_inserir_aresta(g, u, v);
}
 
/* Repete o algoritmo ate passar ~20 ms e devolve o tempo medio em microssegundos
   (um unico uso e rapido demais para o clock() medir). */
static double medir_us(Algoritmo alg, const GrafoCobertura *g) {
    int cobertura[MAX_V];
    long repeticoes = 0;
    clock_t inicio = clock();
    clock_t limite = CLOCKS_PER_SEC / 50;
 
    do {
        alg(g, cobertura);
        repeticoes++;
    } while (clock() - inicio < limite);
 
    double total_us = (double)(clock() - inicio) * 1000000.0 / CLOCKS_PER_SEC;
    return total_us / repeticoes;
}
 
int main(void) {
    /* ---------- Exemplo pequeno ---------- */
    GrafoCobertura g;
    grafo_iniciar(&g, 6);
    grafo_inserir_aresta(&g, 0, 1);
    grafo_inserir_aresta(&g, 0, 2);
    grafo_inserir_aresta(&g, 1, 2);
    grafo_inserir_aresta(&g, 1, 3);
    grafo_inserir_aresta(&g, 2, 4);
    grafo_inserir_aresta(&g, 3, 5);
    grafo_inserir_aresta(&g, 4, 5);
 
    int cobertura[MAX_V];
 
    printf("=== Exemplo: 6 vertices, %d arestas ===\n", grafo_num_arestas(&g));
 
    int tam_bt = cobertura_minima_bt(&g, cobertura);
    printf("Backtracking: tamanho %d  ", tam_bt);
    imprimir_cobertura(&g, cobertura);
    printf("  valida: %s\n", eh_cobertura(&g, cobertura) ? "sim" : "NAO");
 
    int tam_gu = cobertura_gulosa(&g, cobertura);
    printf("Gulosa:       tamanho %d  ", tam_gu);
    imprimir_cobertura(&g, cobertura);
    printf("  valida: %s\n", eh_cobertura(&g, cobertura) ? "sim" : "NAO");
 
    /* Cobertura invalida de proposito, para testar eh_cobertura. */
    int invalida[MAX_V] = {0};
    invalida[0] = 1;
    printf("Teste eh_cobertura({0}): %s (esperado: nao)\n",
           eh_cobertura(&g, invalida) ? "sim" : "nao");
 
    /* ---------- Experimento de tempo ---------- */
    printf("\n=== Experimento: grafos aleatorios (densidade 40%%) ===\n");
    printf("%3s %8s | %5s %12s | %5s %12s | %s\n",
           "n", "arestas", "BT", "tempo(us)", "guloso", "tempo(us)", "guloso foi otimo?");
 
    int tamanhos[] = {5, 8, 10, 12, 15};
    srand(42);
    for (int i = 0; i < 5; i++) {
        int n = tamanhos[i];
        GrafoCobertura h;
        gerar_grafo_aleatorio(&h, n, 40);
 
        int c1[MAX_V], c2[MAX_V];
        int otimo = cobertura_minima_bt(&h, c1);
        int guloso = cobertura_gulosa(&h, c2);
 
        double t_bt = medir_us(cobertura_minima_bt, &h);
        double t_gu = medir_us(cobertura_gulosa, &h);
 
        printf("%3d %8d | %5d %12.2f | %5d %12.2f | %s\n",
               n, grafo_num_arestas(&h), otimo, t_bt, guloso, t_gu,
               guloso == otimo ? "sim" : "nao");
    }
 
    return 0;
}