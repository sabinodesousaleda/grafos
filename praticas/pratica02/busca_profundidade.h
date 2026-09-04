#ifndef BUSCA_PROFUNDIDADE_H
#define BUSCA_PROFUNDIDADE_H

#include "../pratica01/grafo_lista.h"

void busca_em_profundidade(GrafoLista *g, int inicio, int *visitado);
int contar_componentes(GrafoLista *g);
int tem_ciclo(GrafoLista *g);
int eh_bipartido(GrafoLista *g);

#endif