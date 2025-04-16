#include <stdbool.h>
#include <string.h>

#define MAXNUMVERTICES 100
#define AN -1 /*aresta nula*/
#define VERTICE_INVALIDO -1 /*vertice invalido ou ausente*/

typedef float Peso;
typedef struct {
    Peso mat[MAXNUMVERTICES][MAXNUMVERTICES];
    int numVertices;
    int numArestas;
} Grafo;
typedef int Apontador;

bool inicializaGrafo(Grafo* g, int nv);
int obtemNrVertices(Grafo* g);
int obtemNrArestas(Grafo* g);
bool verificaValidadeVertice(int v, Grafo* g);
void insereAresta(int v1, int v2, Peso peso, Grafo* g);
bool existeAresta(int v1, int v2, Grafo* g);
Peso obtemPesoAresta(int v1, int v2, Grafo* g);
bool removeArestaObtPeso(int v1, int v2, Peso* peso, Grafo* g);
bool removeAresta(int v1, int v2, Grafo* g);
bool listaAdjVazia(int v, Grafo* g);
Apontador primeiroListaAdj(int v, Grafo* g);
Apontador proxListaAdj(int v, Grafo* g, Apontador atual);
void imprimeGrafo(Grafo* g);
void liberaGrafo(Grafo* g);
int verticeDestino(Apontador p, Grafo* g);
