#include <stdio.h>
#include <stdlib.h>
#include "grafo_listaadj.h"

bool inicializaGrafo (Grafo* g, int nv) {
    if (g == NULL) {
        fprintf(stderr, "ERRO na chamada de inicializaGrafo: grafo = NULL.\n");
        return false;
    }
    if (nv <= 0) {
        fprintf(stderr, "ERRO na chamada de inicializaGrafo: numero de vertices deve ser positivo.\n");
        return false;
    }
    g->numVertices = nv;
    if (!(g->listaAdj = (Apontador*) calloc(nv, sizeof(Apontador)))) {
        fprintf(stderr, "ERRO: Falha na alocacao de memoria na funcao inicializaGrafo.\n");
        return false;
    }
    g->numArestas = 0;
    //calloc ja inicializa com zeros... nao precisa inicializar g->listaAdj[i]
    return true;
}

int obtemNrVertices(Grafo* g) {
    return (g->numVertices);
 }

int obtemNrArestas(Grafo* g) {
    return (g->numArestas);
}

bool verificaValidadeVertice (int v, Grafo* g) {
    if (g == NULL) {
        fprintf(stderr, "ERRO: grafo = NULL.\n");
        return false;
    }
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

/* NAO verifica se a aresta ja existia. Insercao SEM ordenacao. Grafo NAO DIRECIONADO */
void insereAresta(int v1, int v2, Peso peso, Grafo* g) {
    Apontador p, q;
    if (!(verificaValidadeVertice(v1,g) && verificaValidadeVertice(v2,g)))
        return;
    if (v1 == v2) return;
    if (!(p= (Apontador) calloc(1,sizeof(Aresta)))) {
        fprintf(stderr, "ERRO: Falha na alocacao de memoria na funcao insereAresta\n");
        return;
    }
    if (!(q= (Apontador) calloc(1,sizeof(Aresta)))) {
        fprintf(stderr, "ERRO: Falha na alocacao de memoria na funcao insereAresta\n");
        return;
    }
    p->vdest = v2;
    p->peso = peso;
    p->prox = g->listaAdj[v1];
    g->listaAdj[v1] = p;

    q->vdest = v1;
    q->peso = peso;
    q->prox = g->listaAdj[v2];
    g->listaAdj[v2] = q;

    g->numArestas++;
}

/* Remove a aresta (v1, v2) do grafo. Se a aresta existia, coloca o peso dessa aresta em "peso" e retorna true.
Para grafos NAO DIRECIONADOS, remove tambem a mesma aresta para o outro vertice.
Caso contrario retorna false (e "peso" eh inalterado) */
bool removeArestaObtPeso(int v1, int v2, Peso* peso, Grafo* g) {
    Apontador q, ant;
    if (!(verificaValidadeVertice(v1,g) && verificaValidadeVertice(v2,g)))
        return false;
    if (v1 == v2) return false;
    q = g->listaAdj[v1];
    while ((q != NULL) && (q->vdest != v2)) {
        ant = q;
        q = q->prox;
    }
    //aresta existe
    if (q != NULL) {
        if (g->listaAdj[v1] == q)
            g->listaAdj[v1] = q->prox;
        else ant->prox = q->prox;
        *peso = q->peso;
        q->prox = NULL; 
        q = NULL;
        free(q);

        Apontador r, ante;
        r = g->listaAdj[v2];
        while ((r != NULL) && (r->vdest != v1)) {
            ante = r;
            r = r->prox;
        }
        if (g->listaAdj[v2] == r) g->listaAdj[v2] = r->prox;
        else ante->prox = r->prox;
        r->prox = NULL;
        r = NULL;
        free(r);

        g->numArestas--;
        return true;
    }
    //aresta nao existe
    return false;
}

bool removeAresta(int v1, int v2, Grafo* g) {
    Apontador q, ant;
    if (!(verificaValidadeVertice(v1,g) && verificaValidadeVertice(v2,g)))
        return false;
    if (v1 == v2) return false;
    q = g->listaAdj[v1];
    while ((q != NULL) && (q->vdest != v2)) {
        ant = q;
        q = q->prox;
    }
    //aresta existe
    if (q != NULL) {
        if (g->listaAdj[v1] == q)
            g->listaAdj[v1] = q->prox;
        else ant->prox = q->prox;
        q->prox = NULL;
        q = NULL;
        free(q);

        Apontador r, ante;
        r = g->listaAdj[v2];
        while ((r != NULL) && (r->vdest != v1)) {
            ante = r;
            r = r->prox;
        }
        if (g->listaAdj[v2] == r) g->listaAdj[v2] = r->prox;
        else ante->prox = r->prox;
        r->prox = NULL;
        r = NULL;
        free(r);

        g->numArestas--;
        return true;
    }
    //aresta nao existe
    return false;
}

bool existeAresta (int v1, int v2, Grafo* g) {
    Apontador q;
    if (!(verificaValidadeVertice(v1,g) && verificaValidadeVertice(v2,g)))
        return false;

    q = g->listaAdj[v1];
    while ((q != NULL) && (q->vdest != v2))
    q = q->prox;
    if (q != NULL) return true;
    return false;
}

/*
Peso obtemPesoAresta(int v1, int v2, Grafo* g):
Retorna o peso da aresta (v1, v2) se ela existir e AN caso contrario.
*/
Peso obtemPesoAresta (int v1, int v2, Grafo* g) {
    if (!(verificaValidadeVertice(v1,g) && verificaValidadeVertice(v2,g)))
        return AN;
    Apontador q;
    q = g->listaAdj[v1];
    while ((q != NULL) && (q->vdest != v2))
    q = q->prox;
    if (q != NULL) return (q->peso);
    return AN;
}

/* bool listaAdjVazia (int v, Grafo* g):
Retorna true se a lista de adjacencia (de vertices adjacentes) do vertice v eh vazia, e false c.c. */
bool listaAdjVazia (int v, Grafo* g) {
    if (! verificaValidadeVertice(v, g))
    return false;
return (g->listaAdj[v]==NULL);
}

/* Apontador proxListaAdj (int v, Grafo* g, Apontador atual):
Retorna o proximo vertice adjacente a v, partindo do vertice "atual" adjacente a v ou NULL se a lista de adjacencia tiver terminado sem um novo proximo.
*/
Apontador proxListaAdj (int v, Grafo* g, Apontador atual) {
    if (! verificaValidadeVertice(v, g))
    return false;
    if (atual == NULL) {
        return VERTICE_INVALIDO;
    }
    return (atual->prox);
}

/* Apontador primeiroListaAdj (v, g): retorna o endereco do primeiro vertice adjacente a v. */
Apontador primeiroListaAdj (int v, Grafo* g) {
    return (g->listaAdj[v]);
}

/* void imprimeGrafo (Grafo* g):
Imprime os vertices e arestas do grafo no seguinte formato
v1: (adj11, peso11); (adj12, peso12); ...
v2: (adj21, peso21); (adj22, peso22); ...
Assuma que cada vertice eh um inteiro de ate 2 digitos. */
void imprimeGrafo(Grafo* g) {
    int v;
    Apontador p;
    for (v = 0; v < g->numVertices; v++) {
        printf("%d: ", v);
        p = g->listaAdj[v];
        while (p != NULL) {
            printf("(%d, %.2f); ", p->vdest, p->peso);
            p = p->prox;
        }
        printf("\n");
    }
}

int verticeDestino(Apontador p, Grafo* g) {
    return (p->vdest);
}

/* Libera o espaco ocupado por um grafo */
void liberaGrafo(Grafo* g) {
    int v;
    Apontador p;
/* libera a lista de adj. de cada vertice */
    for (v = 0; v < g->numVertices; v++) {
        while ((p = g->listaAdj[v]) != NULL) {
            g->listaAdj[v] = p->prox;
            p->prox = NULL;
            free(p);
        }
    }
    g->numVertices = 0;
    g->numArestas = 0;
    g->listaAdj = NULL;
    free(g->listaAdj);
}
