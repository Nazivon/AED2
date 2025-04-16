#include <stdio.h>
#include <stdlib.h>
#include "grafo_matrizadj.h"

/*
 InicializaGrafo(Grafo* g, int nv): inicializa um grafo com nv vertices
 Vertices vao de 1 a nv
 Preenche as celulas com AN (representando ausencia de aresta)
 Retorna true se inicializou com sucesso e false caso contrário
 */


 bool inicializaGrafo(Grafo* g, int nv) {
    int i, j;
    if (nv > MAXNUMVERTICES) {
        fprintf(stderr, "ERRO na chamada de inicializaGrafo: Numero de vertices maior que o maximo permitido de %d.\n", MAXNUMVERTICES);
        return false;
    }
    if (nv <= 0) {
        fprintf(stderr, "ERRO na chamada de inicializaGrafo: Numero de vertices deve ser positivo.\n");
        return false;
    }
    g->numVertices = nv;
    g->numArestas = 0;
    for (i = 0; i < g->numVertices; i++) {
        for (j = 0; j < g->numVertices; j++)
        g->mat[i][j] = AN;
    }
    return true;
 }

int obtemNrVertices(Grafo* g) {
    return (g->numVertices);
 }

int obtemNrArestas(Grafo* g) {
    return (g->numArestas);
}

/* void imprimeGrafo (Grafo* g):
Imprime os vertices e arestas do grafo no seguinte formato
v1: (adj11, peso11); (adj12, peso12); ...
v2: (adj21, peso21); (adj22, peso22); ...
Assuma que cada vertice eh um inteiro de ate 2 digitos. */
void imprimeGrafo(Grafo* g) {
    int i, j;
    for (i = 0; i < g->numVertices; i++) {
        printf("%d: ", i);
        for (j = 0; j < g->numVertices; j++) {
            if (g->mat[i][j] != AN) {
                printf("(%d, %.2f); ", j, g->mat[i][j]);
            }
        }
        printf("\n");
    }
 }

 bool verificaValidadeVertice (int v, Grafo* g) {
    if (v > g->numVertices) {
        fprintf(stderr, "ERRO: Numero do vertice (%d) maior ou igual que o numero total de vertices (%d).\n", v, g->numVertices);
        return false;
    }
    if (v < 0) {
        fprintf(stderr, "ERRO: Numero do vertice (%d) deve ser nao negativo.\n", v);
        return false;
    }
    return true;
}

/*
 void insereAresta(int v1, int v2, Peso peso, Grafo* g): 
 Insere a aresta com peso "peso" no grafo NAO DIRECIONADO.
 Nao verifica se a aresta ja existia (isso deve ser feito pelo usuario antes, se necessario).
 */
 void insereAresta(int v1, int v2, Peso peso, Grafo* g) {
    if(!(verificaValidadeVertice(v1,g) && verificaValidadeVertice(v2,g)))
    return;
if (v1 == v2) return;
g->mat[v1][v2] = peso;
g->mat[v2][v1] = peso;
g->numArestas++;
return;
}

/*
bool existeAresta(int v1, int v2, Grafo* g):
Retorna true se existe a aresta (v1, v2) no grafo e false caso contrario
*/
bool existeAresta (int v1, int v2, Grafo* g) {
    if (!(verificaValidadeVertice(v1,g) && verificaValidadeVertice(v2,g)))
        return false;
    if (g->mat[v1][v2] != AN) return true;
    else return false;
}

/*
Peso obtemPesoAresta(int v1, int v2, Grafo* g):
Retorna o peso da aresta (v1, v2) se ela existir e AN caso contrario.
*/
Peso obtemPesoAresta (int v1, int v2, Grafo* g) {
    if (!(verificaValidadeVertice(v1,g) && verificaValidadeVertice(v2,g)))
        return VERTICE_INVALIDO;
    return (g->mat[v1][v2]);
}

/*
bool removeArestaObtPeso (int v1, int v2, Peso* peso, Grafo* g):
remove a aresta (v1, v2) do grafo colocando AN em sua celula.
Se a aresta existia, coloca o peso dessa aresta em "peso" e retorna true.
Caso contrario retorna false (e peso eh inalterado).
*/
bool removeArestaObtPeso (int v1, int v2, Peso* peso, Grafo* g) {
    if (!(verificaValidadeVertice(v1, g) && verificaValidadeVertice(v2, g)))
    return false;
/*se aresta existe*/
if (g->mat[v1][v2] != AN) {
    *peso = g->mat[v1][v2];
    g->mat[v1][v2] = AN;
    g->mat[v2][v1] = AN;
    g->numArestas--;
    return true;
}
/*aresta nao existe*/
return false;
}

bool removeAresta(int v1, int v2, Grafo* g) {
    if (!(verificaValidadeVertice(v1, g) && verificaValidadeVertice(v2, g)))
    return false;
/*se aresta existe*/
if (g->mat[v1][v2] != AN) {
    g->mat[v1][v2] = AN;
    g->mat[v2][v1] = AN;
    g->numArestas--;
    return true;
}
/*aresta nao existe*/
return false;
}

/*
bool listaAdjVazia (int v, Grafo* g):
retorna true se a lista de adjacencia (vertices adjacentes) do vertice v for vazia, e false caso contrario.
*/
bool listaAdjVazia (int v, Grafo* g) {
    if (!verificaValidadeVertice(v, g))
    return true;
int i;
for (i = 0; i < g->numVertices; i++)
    if (g->mat[v][i] != AN) return false;
return true;
}

/*
Apontador proxListaAdj (int v, Grafo* g, Apontador atual):
Trata-se de um iterador sobre a lista de adjacencia do vertice v.
Retorna o proximo vertice adjacente a v, partindo do vertice "atual" adjacente a v ou VERTICE_INVALIDO se a lista de adjacencia tiver terminado sem um novo proximo.
*/
Apontador proxListaAdj (int v, Grafo* g, Apontador atual) {
    if (!verificaValidadeVertice(v, g))
    return VERTICE_INVALIDO;

atual++;
while ((atual < g->numVertices) && (g->mat[v][atual] == AN))
atual++;
if (atual >= g->numVertices) {
    return VERTICE_INVALIDO;
}
return atual;
}

Apontador primeiroListaAdj (int v, Grafo* g) {
    return (proxListaAdj (v, g, -1));
}

int verticeDestino (Apontador p, Grafo* g) {
    return p;
}

void liberaGrafo(Grafo* g){}
