#ifdef MATRIZ
 #include "grafo_matrizadj.h"
 #else
 #include "grafo_listaadj.h"
 #endif

#include <stdio.h>
#include <stdlib.h>

#define MAX_FILENAME 256
/* 
leGrafo (nomearq, Grafo)
Le o arquivo nomearq e armazena na estrutura Grafo
Layout do arquivo:
    A 1a linha deve conter o numero de vertices e o numero de arestas do grafo, separados por espaco.
    A 2a linha em diante deve conter a informacao de cada aresta, q consiste no indice do vertice de origem, indice do vertice de destino
    e o peso da aresta, tambem separados por espacos.
    Observacoes: os vertices devem ser ordenados de 0 a v-1.
    Os pesos das arestas sao numeros racionais nao negativos.

    Exemplo: O arquivo abaixo contem um grafo com 4 vertices (0,1,2,3) e 7 arestas.

    4 7
    0 3 6.3
    2 1 5.0
    2 0 9
    1 3 1.7
    0 1 9
    3 1 5.6
    0 2 7.2
    
    Codigo de saida:
    1: leitura bem sucedida
    0: erro na leitura do arquivo
*/
int leGrafo (char* nomearq, Grafo* grafo) {
    FILE* fp;
    int nvertices, narestas;
    int v1, v2;
    Peso peso;

    fp = fopen(nomearq, "r");
    if (fp == NULL){
        fprintf(stderr, "ERRO: arquivo NULL.\n");
        return 0;
}
if (fscanf(fp, "%d %d", &nvertices, &narestas) != 2) {
    fprintf(stderr, "ERRO: devem haver dois valores na 1a linha - o no. de vertices e o no. de arestas do grafo.\n");
        return 0;
}

inicializaGrafo (grafo, nvertices);

while (fscanf(fp, "%d %d %f", &v1, &v2, &peso) == 3) {
insereAresta(v1, v2, peso, grafo);
}

fclose(fp);
return 1;
}

typedef struct {
    int indice;
    int valor;
} IndexAux;

typedef struct estr {
    int chave;
    struct estr* prox;
} NO;

typedef struct {
    NO* topo;
} Pilha;

typedef struct {
    NO* inicio;
    NO* fim;
} Fila;

void visitaBP (int v, Grafo* g, int* tempo, char* cor, int* tdesc, int* tterm, int* antecessor, int* baixo) {
    cor[v] = 'C'; tdesc[v] = ++(*tempo);

    baixo[v] = tdesc[v];
    Apontador atual;
    for (atual = primeiroListaAdj(v, g); atual != VERTICE_INVALIDO; atual = proxListaAdj(v, g, atual)) {
        int u = verticeDestino(atual, g);
        if (cor[u] == 'B') {
            antecessor[u] = v;
            visitaBP (u, g, tempo, cor, tdesc, tterm, antecessor, baixo);
            baixo[v] = (baixo[v] < baixo[u]) ? baixo[v] : baixo[u];
        }
        baixo[v] = (baixo[v] < tdesc[u]) ? baixo[v] : tdesc[u];
    }
    tterm[v] = ++(*tempo);
    cor[v] = 'P';
}

void inicializaFila (Fila* f) {
    f->inicio = NULL;
    f->fim = NULL;
}

void insereFila (Fila* f, int ch) {
    NO* novo = (NO*) malloc(sizeof(NO));
    novo->chave = ch;
    novo->prox = NULL;
    if (f->fim) f->fim->prox = novo;
    else f->inicio = novo;
    f->fim = novo;
}

int removeFila (Fila* f) {
    NO* auxiliar;
    if (!f->inicio) return -1;
    auxiliar = f->inicio;
    int ch = auxiliar->chave;
    f->inicio = f->inicio->prox;
    free(auxiliar);
    if(!f->inicio) f->fim = NULL;
    return ch;
}

void visitaBL (int v, Grafo* g, FILE* Output, char* color, int* ante, int* distancia) {
    color[v] = 'C'; distancia[v] = 0;
    Fila f1;
    inicializaFila(&f1);
    insereFila(&f1, v);
    while (f1.inicio != NULL) {
        int w = removeFila(&f1);
        fprintf(Output, "%d ", w);
        Apontador atual;
        for (atual = primeiroListaAdj(w, g); atual != VERTICE_INVALIDO; atual = proxListaAdj(w, g, atual)) {
            int u = verticeDestino(atual, g);
            if (color[u] == 'B') {
                color[u] = 'C';
                ante[u] = w;
                distancia[u] = distancia[w] + 1;
                insereFila (&f1, u);
            }
        }
        color[w] = 'P';
    }
}

void insertionSort(IndexAux* arranjo, int comeco, int n, int (*compare)(IndexAux, IndexAux)) {
    int i, j;
    IndexAux chave;
    for (i = comeco; i < (comeco + n - 1); i++) {
        chave.valor = arranjo[i].valor;
        chave.indice = arranjo[i].indice;
        j = i - 1;
        while (j >= comeco-1 && compare(arranjo[j], chave) > 0) {
            arranjo[j+1] = arranjo[j];
            j = j - 1;
        }
        arranjo[j + 1] = chave;
    }
}

//Funcoes para comparar o arranjo no insertionSort pelo valor ou pelo indice
int compareValor(IndexAux a, IndexAux b) {
    return a.valor - b.valor;
}

int compareIndice(IndexAux a, IndexAux b) {
    return a.indice - b.indice;
}

void inicializaPilha (Pilha* p) {
    p->topo = NULL;
}

NO* busca (Pilha* p, int ch) {
    NO* nodo = p->topo;
    while (nodo) {
        if (nodo->chave == ch) return nodo;
        nodo = nodo->prox;
    }
    return NULL;
}

void pushP (int ch, Pilha* p) {
    NO* atual = busca(p, ch);
    if (atual) return;
    NO* novo = (NO*) malloc(sizeof(NO));
    novo->chave = ch;
    novo->prox = p->topo;
    p->topo = novo;
}

void liberaPilha (Pilha* p) {
    while (p->topo != NULL) {
        NO* aux = p->topo;
        p->topo = p->topo->prox;
        free(aux);
    }
}

void imprimePilha (Pilha* p, FILE* Output) {
    NO* nodo = p->topo;
    while (nodo) {
        fprintf(Output, "%d ", nodo->chave);
        nodo = nodo->prox;
    }
}

void buscaLargura (Grafo* g, FILE* Output, int* ante) {
    char color[g->numVertices];

    int distancia[g->numVertices];
    for (int v = 0; v < g->numVertices; v++) {
        color[v] = 'B';
        ante[v] = -1;
        distancia[v] = 2147483647;
    }
    for (int v = 0; v < g->numVertices; v++) {
        if (color[v] == 'B') visitaBL(v, g, Output, color, ante, distancia);
    }
}

void buscaProfundidade (Grafo* g, FILE* Output, int* tdesc, int* antecessor, int* baixo) {
    char cor[g->numVertices];
    
    int tterm[g->numVertices];

    int tempo = 0;

    for (int v = 0; v < g->numVertices; v++) {
        cor[v] = 'B';
        tdesc[v] = tterm[v] = 0;
        antecessor[v] = -1;
    }
    for (int v = 0; v < g->numVertices; v++) {
        if (cor[v] == 'B') visitaBP(v, g, &tempo, cor, tdesc, tterm, antecessor, baixo);
    }
}

void imprimeCaminhoBL (Grafo* g, FILE* Output) {
    int ante[g->numVertices];
    buscaLargura(g, Output, ante);

    fprintf(Output, "\n\n");
    fprintf(Output, "Caminhos BL:\n");
    Pilha p2;
    inicializaPilha(&p2);

    for (int i = 0; i < g->numVertices; i++) {
        pushP(i, &p2);
        int j = i;
        while (ante[j] != -1) {
            pushP(ante[j], &p2);
            j =  ante[j];
        }
        imprimePilha(&p2, Output);
        fprintf(Output, "\n");
        liberaPilha(&p2);
    }
    fprintf(Output, "\n");
}

void imprimeCaminhoBP (Grafo* g, FILE* Output) {
    int tdesc[g->numVertices];
    int antecessor[g->numVertices];
    int baixo[g->numVertices];

    buscaProfundidade(g, Output, tdesc, antecessor, baixo);
    IndexAux indiceAux[g->numVertices];
    for (int i = 0; i < g->numVertices; i++) {
        indiceAux[i].indice = i;
        indiceAux[i].valor = tdesc[i];
    }
    insertionSort (indiceAux, 1, g->numVertices, compareValor);
    for (int i = 0; i < g->numVertices; i++) {
        fprintf(Output, "%d ", indiceAux[i].indice);
    }

    fprintf(Output, "\n\n");
    fprintf(Output, "Caminhos BP:\n");
    Pilha p1;
    inicializaPilha(&p1);

    Pilha pArticula;
    inicializaPilha(&pArticula);

    for (int i = 0; i < g->numVertices; i++) {
        pushP(i, &p1);
        int j = i;
        while (antecessor[j] != -1) {
            pushP(antecessor[j], &p1);
            if (tdesc[antecessor[j]] <= baixo[j] && antecessor[antecessor[j]] != -1) {
                pushP(antecessor[j], &pArticula);
            }
            j = antecessor[j];
        }
        imprimePilha(&p1, Output);
        fprintf(Output, "\n");
        liberaPilha(&p1);
    }
    fprintf(Output, "\n");

    int x = 1;
    int comeco = 0;
    fprintf(Output, "Componentes Conectados:\n");
    for (int i = 0; i < g->numVertices ; i++) {
        int k = indiceAux[i].indice;
        if (antecessor[k] == -1) {
	        int contador = 1;
            fprintf(Output, "C%d: %d ", x, k);
            int j;
            if ((i+1) < g->numVertices) {
 	            int filhosR = 0;
                for (j = i+1; j < g->numVertices && antecessor[indiceAux[j].indice] != -1; j++) {
                    contador++;
                    if (antecessor[indiceAux[j].indice] == k) filhosR++;
                }
                if (filhosR > 1) pushP(k, &pArticula);
                insertionSort(indiceAux, comeco+1, contador, compareIndice);
                for (int i = comeco+1; i < comeco+contador; i++) {
                    fprintf(Output, "%d ", indiceAux[i].indice);
                }
            }
            x++;
            comeco += contador;
            fprintf(Output, "\n");
        }
    }
fprintf(Output, "\n");
fprintf(Output, "Vertices de articulacao:\n");

imprimePilha(&pArticula, Output);
liberaPilha(&pArticula);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Este programa exige dois argumentos: o arquivo de entrada e o de saida.\n");
        exit(1);
    }
    printf("%s ", argv[0]);
    printf("%s ", argv[1]);
    printf("%s\n", argv[2]);

    Grafo g1;
    char filename[MAX_FILENAME] = "";
    strcpy(filename, argv[1]);
    if (!leGrafo (filename, &g1)) {
        fprintf(stderr, "Erro ao ler o grafo do arquivo.\n");
    }

    //imprimeGrafo(&g1);

    char saida[MAX_FILENAME] = "";
    strcpy(saida, argv[2]);

    FILE *Output = fopen(saida, "w");
    if (Output == NULL) {
    printf("Erro ao abrir o arquivo de saida.\n");
    return 1;
    }

    fprintf(Output, "BL:\n");
    imprimeCaminhoBL(&g1, Output);

    fprintf(Output, "BP:\n");
    imprimeCaminhoBP(&g1, Output);

    fclose(Output);

    return 0;
}
