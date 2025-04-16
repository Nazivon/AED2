#include <stdbool.h>
#include <string.h>

#define VERTICE_INVALIDO NULL /* no. do vertice invalido ou ausente */
#define AN -1 /* aresta nula */

typedef float Peso;

/* tipo estrutura Aresta : vertice destino, peso, ponteiro p/ prox. aresta */
typedef struct str_aresta {
    int vdest;
    Peso peso;
    struct str_aresta * prox;
} Aresta;
typedef Aresta* Apontador;
/* tipo estrutura grafo :
 listaAdj: vetor de Arestas ligadas (cada posicao i contem o ponteiro p/ o inicio da lista de adj. do vertice i)
 */
typedef struct {
    Apontador* listaAdj;
    int numVertices;
    int numArestas;
} Grafo;

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
