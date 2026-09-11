#include <stdio.h>
#include <stdlib.h>

#include "../pratica01/grafo_lista.h"
#include "busca_largura.h"
#include "busca_profundidade.h"

int main(void)
{
    GrafoLista *g = criar_grafo_lista(6);

    if (g == NULL) {
        printf("Erro ao criar o grafo.\n");
        return 1;
    }

    /*
     * Grafo:
     *
     *     1 ----- 3
     *    /       /
     *   0 ----- 2
     *
     *     4 ----- 5
     *
     * Possui 2 componentes conexos.
     * Não possui ciclo.
     * É bipartido.
     */

    inserir_aresta_lista(g, 0, 1);
    inserir_aresta_lista(g, 0, 2);
    inserir_aresta_lista(g, 1, 3);
    inserir_aresta_lista(g, 2, 3);
    inserir_aresta_lista(g, 4, 5);

    printf("=== BFS ===\n");

    int *dist = malloc(g->n * sizeof(int));
    int *pred = malloc(g->n * sizeof(int));

    if (dist == NULL || pred == NULL) {
        printf("Erro ao alocar memoria.\n");

        free(dist);
        free(pred);
        liberar_grafo_lista(g);

        return 1;
    }

    bfs(g, 0, dist, pred);

    for (int i = 0; i < g->n; i++) {
        printf(
            "Vertice %d: distancia = %d, predecessor = %d\n",
            i,
            dist[i],
            pred[i]
        );
    }

    printf("\n=== DFS ===\n");

    int *visitado = calloc(g->n, sizeof(int));
    int *entrada = malloc(g->n * sizeof(int));
    int *saida = malloc(g->n * sizeof(int));

    if (visitado == NULL || entrada == NULL || saida == NULL) {
        printf("Erro ao alocar memoria para DFS.\n");

        free(dist);
        free(pred);
        free(visitado);
        free(entrada);
        free(saida);
        liberar_grafo_lista(g);

        return 1;
    }

    int tempo = 0;

    for (int i = 0; i < g->n; i++) {
        if (!visitado[i]) {
            dfs_recursiva(
                g,
                i,
                visitado,
                &tempo,
                entrada,
                saida
            );
        }
    }

    for (int i = 0; i < g->n; i++) {
        printf(
            "Vertice %d: entrada = %d, saida = %d\n",
            i,
            entrada[i],
            saida[i]
        );
    }

    printf("\n=== Propriedades do grafo ===\n");

    printf(
        "Numero de componentes conexos: %d\n",
        contar_componentes(g)
    );

    printf(
        "Possui ciclo? %s\n",
        tem_ciclo(g) ? "Sim" : "Nao"
    );

    printf(
        "E bipartido? %s\n",
        eh_bipartido(g) ? "Sim" : "Nao"
    );

    free(dist);
    free(pred);
    free(visitado);
    free(entrada);
    free(saida);

    liberar_grafo_lista(g);

    return 0;
}