#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "hamiltoniano.h"
#include "tps.h"

static void imprime(const int v[], int k) {
    for (int i = 0; i < k; i++) printf("%d%s", v[i], i < k - 1 ? " -> " : "");
    printf("\n");
}

static double ms(clock_t ini) {
    return 1000.0 * (double)(clock() - ini) / CLOCKS_PER_SEC;
}

/* Grafo aleatório com probabilidade p (em %) para cada aresta. */
static void grafo_aleatorio(GrafoH *g, int n, int p) {
    iniciar_grafo_h(g, n);
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            if (rand() % 100 < p) inserir_aresta_h(g, i, j);
}

int main(void) {
    int r[MAX_V + 1];

    /* ---- Parte 1: Hamiltoniano num grafo pequeno ---- */
    GrafoH g;
    iniciar_grafo_h(&g, 6);
    int arestas[][2] = {{0,1},{1,2},{2,3},{3,4},{4,5},{5,0},{0,3},{1,4}};
    for (int i = 0; i < 8; i++) inserir_aresta_h(&g, arestas[i][0], arestas[i][1]);

    printf("=== Hamiltoniano (grafo com 6 vertices) ===\n");
    if (encontrar_caminho_hamiltoniano_bt(&g, r)) { printf("Caminho: "); imprime(r, g.n); }
    else printf("Nao ha caminho hamiltoniano.\n");
    if (encontrar_ciclo_hamiltoniano_bt(&g, r)) {
        r[g.n] = r[0];
        printf("Ciclo:   "); imprime(r, g.n + 1);
    } else printf("Nao ha ciclo hamiltoniano.\n");

    /* Grafo estrela: sem caminho hamiltoniano */
    GrafoH s;
    iniciar_grafo_h(&s, 5);
    for (int i = 1; i < 5; i++) inserir_aresta_h(&s, 0, i);
    printf("Estrela (5 vertices): caminho=%s, ciclo=%s\n",
           encontrar_caminho_hamiltoniano_bt(&s, r) ? "sim" : "nao",
           encontrar_ciclo_hamiltoniano_bt(&s, r) ? "sim" : "nao");

    /* ---- Parte 2: TSP via redução ---- */
    InstanciaTSP t;
    grafo_para_tsp(&g, &t);
    printf("\n=== TSP (grafo de 6 vertices, pesos 1/2) ===\n");
    int c = tsp_forca_bruta(&t, r);
    printf("Forca bruta: custo %d | ", c); imprime(r, t.n + 1);
    c = tsp_vizinho_mais_proximo(&t, r);
    printf("Vizinho:     custo %d | ", c); imprime(r, t.n + 1);
    c = tsp_insercao(&t, r);
    printf("Insercao:    custo %d | ", c); imprime(r, t.n + 1);

    /* ---- Parte 3: experimento ---- */
    printf("\n=== Experimento (grafos aleatorios, 50%% de arestas) ===\n");
    printf("%3s | %-18s | %-18s | %-18s\n", "n", "Forca bruta", "Vizinho", "Insercao");
    printf("%3s | %8s %9s | %8s %9s | %8s %9s\n", "", "custo", "ms", "custo", "ms", "custo", "ms");
    int tams[] = {5, 8, 10, 12};
    srand(42);
    for (int k = 0; k < 4; k++) {
        GrafoH h;
        InstanciaTSP ti;
        grafo_aleatorio(&h, tams[k], 50);
        grafo_para_tsp(&h, &ti);

        clock_t ini = clock();
        int c1 = tsp_forca_bruta(&ti, r);
        double t1 = ms(ini);
        ini = clock();
        int c2 = tsp_vizinho_mais_proximo(&ti, r);
        double t2 = ms(ini);
        ini = clock();
        int c3 = tsp_insercao(&ti, r);
        double t3 = ms(ini);

        printf("%3d | %8d %9.3f | %8d %9.3f | %8d %9.3f\n",
               tams[k], c1, t1, c2, t2, c3, t3);
    }
    return 0;
}