#include <stdio.h>
#include <stdlib.h>
#include "../pratica01/grafo_lista.h"
#include "busca_largura.h"
#include "busca_profundidade.h"

int main(void) {
    GrafoLista *g = criar_grafo_lista(6);

    inserir_aresta_lista(g, 0, 1);
    inserir_aresta_lista(g, 0, 2);
    inserir_aresta_lista(g, 1, 3);
    inserir_aresta_lista(g, 2, 3);
    inserir_aresta_lista(g, 4, 5);

    printf("=== Teste de Busca em Largura (BFS) ===\n");
    int *dist = busca_em_largura(g, 0);
    if (dist) {
        for (int i = 0; i < g->n; i++) {
            printf("Distância de 0 a %d: %d\n", i, dist[i]);
        }
        free(dist);
    }

    printf("\n=== Teste de Propriedades do Grafo (DFS/BFS) ===\n");
    printf("Número de componentes conexas: %d\n", contar_componentes(g));
    printf("Possui ciclo? %s\n", tem_ciclo(g) ? "Sim" : "Não");
    printf("É bipartido? %s\n", eh_bipartido(g) ? "Sim" : "Não");

    liberar_grafo_lista(g);
    return 0;
}
