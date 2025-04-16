#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define t 3
#define MAX_FILENAME 256

typedef int TipoChave;

typedef enum {
    interno, folha
} TipoNO;

typedef struct NO NO;

typedef struct {
    TipoChave* registros[2*t-1];
    NO* prox;
} NOfolha;

typedef struct {
    NO* filhos[2*t];
} NOint;

struct NO {
    TipoNO tipo;
    NOint noInterno;
    NOfolha noFolha;
    TipoChave chave[2*t-1];
    int numChaves;
};

typedef struct {
    NO* raiz;
} ArvBmais;

NO* criaNoInterno() {
    NO* novoNo = (NO*) malloc(sizeof(NO));
    if (novoNo) {
        novoNo->tipo = interno;
        novoNo->numChaves = 0;
    } return novoNo;
}

NO* criaNoFolha () {
    NO* novoNo = (NO*) malloc(sizeof(NO));
    if (novoNo) {
        novoNo->tipo = folha;
        novoNo->numChaves = 0;
        novoNo->noFolha.prox = NULL;
        for (int i = 0; i < 2*t-1; i++) {
            novoNo->noFolha.registros[i] = NULL;
        }
    } return novoNo;
}

bool criaArvoreB (ArvBmais* T) {
    NO* x = criaNoFolha();
    if (!x) {
        printf("Alocacao de memoria falhou!"); 
        return false;
    }
    T->raiz = x;
    return true;
}

void splitFilhoArvB (NO* x, int i, NO* y) {
    NO* z;
    if (y->tipo == interno) {
        z = criaNoInterno();
        z->numChaves = t-1;
        for (int j = 0; j < t-1; j++) {
            z->chave[j] = y->chave[j+t];
        }
        for (int j = 0; j < t; j++) {
            z->noInterno.filhos[j] = y->noInterno.filhos[j+t];
        }
    } else {
        z = criaNoFolha();
        y->noFolha.prox = z;
        z->numChaves = t;
        for (int j = 0; j < t; j++) {
            z->chave[j] = y->chave[j+t-1];
        }
    }
    y->numChaves = t-1;
    for (int j = x->numChaves; j > i; j--) {
        x->noInterno.filhos[j+1] = x->noInterno.filhos[j];
    }
    x->noInterno.filhos[i+1] = z;
    for (int j = x->numChaves; j > i; j--) {
        x->chave[j] = x->chave[j-1];
    }
    x->chave[i] = y->chave[t-1];
    (x->numChaves)++;
}

void insereNCheioArvB (NO* x, TipoChave k) {
    int i = x->numChaves - 1;
    if (x->tipo == folha) {
        while (i >= 0 && k < x->chave[i]) {
            x->chave[i+1] = x->chave[i];
            i--;
        }
        x->chave[i+1] = k;
        x->numChaves++;
    } else {
        while (i >= 0 && k < x->chave[i]) {
            i--;
        }
        i++;
        NO* ver = x->noInterno.filhos[i];
        if (ver->numChaves == 2*t - 1) {
            splitFilhoArvB(x, i, ver);
            if (k > x->chave[i]) i++;
        }
        insereNCheioArvB(x->noInterno.filhos[i], k);
    }
}

void insereArvoreB (ArvBmais* T, TipoChave k) {
    if(!T) {
        printf("Erro: nenhuma arvore.");
        return;
    }
    NO* r = T->raiz;
    if (r->numChaves == 2*t-1) {
        NO* s = criaNoInterno();
        T->raiz = s;
        s->noInterno.filhos[0] = r;
        splitFilhoArvB(s, 0, r);
        insereNCheioArvB(s, k);
    } else insereNCheioArvB(r, k);
}

NO* min (NO* r) {
    if (r == NULL) return NULL;
    while (r->tipo == interno) {
        r = r->noInterno.filhos[0];
    }
    return r;
}

NO* max (NO* r) {
    if (r == NULL) return NULL;
    while (r->tipo == interno) {
        r = r->noInterno.filhos[r->numChaves];
    }
    return r;
}

void mergeFilhoArvB (NO* x, NO* y, NO* z, int i) {
    if (y->tipo == interno)  { y->chave[t-1] = x->chave[i];
        for (int j = 0; j < t-1; j++) {
            y->chave[j+t] = z->chave[j];
        }
        for (int j = 0; j < t; j++) {
            y->noInterno.filhos[j+t] = z->noInterno.filhos[j];
        }
        y->numChaves = 2*t-1;
    } else if (y->tipo == folha) {
        for (int j = 0; j < t-1; j++) {
            y->chave[j+t-1] = z->chave[j];
        }
        y->numChaves = 2*t-2;
    }
    if (y->tipo == folha) y->noFolha.prox = z->noFolha.prox;
    for (int j = i; j < x->numChaves-1; j++) {
        x->chave[j] = x->chave[j+1];
    }
    for (int j = i; j < x->numChaves-1; j++) {
        x->noInterno.filhos[j+1] = x->noInterno.filhos[j+2];
    }
    (x->numChaves)--;
}

void removeArvBmais (NO* r, TipoChave k) {
    if (r->tipo == interno) {
        int i = 0;
        while ((i < r->numChaves) && (k > r->chave[i])) {
            i++;
        }
        if ((i < r->numChaves) && (k == r->chave[i])) { /* no interno e a chave esta la */
            NO* y = r->noInterno.filhos[i];
            NO* z = r->noInterno.filhos[i+1];
            if (y->numChaves >= t) {
                NO* pred = max(y);
                TipoChave kL = pred->chave[pred->numChaves-1];
                removeArvBmais(y, kL);
                r->chave[i] = kL;
                min(z)->chave[0] = kL;
            } else if (z->numChaves >= t) {
                NO* suc = min(z);
                TipoChave kR = suc->chave[1];
                removeArvBmais(z, kR);
                r->chave[i] = kR;
                suc->chave[0] = kR;
            } else {
                mergeFilhoArvB (r, y, z, i);
                free(z);
                removeArvBmais(y, k);
            }
        } else { /* no interno e a chave nao esta la */
            NO* cam = r->noInterno.filhos[i];
            if (cam->numChaves==t-1) {
                if (i > 0 && (r->noInterno.filhos[i-1])->numChaves >= t) {
                        NO* irmaoAnte = r->noInterno.filhos[i-1];
                        for (int j = cam->numChaves; j > 0; j--) {
                            if (cam->tipo == interno) { cam->noInterno.filhos[j] = cam->noInterno.filhos[j-1]; }
                            cam->chave[j] = cam->chave[j-1]; 
                        }
                        if (cam->tipo == interno) {
                            cam->chave[0] = r->chave[i-1];
                            cam->noInterno.filhos[0] = irmaoAnte->noInterno.filhos[irmaoAnte->numChaves];
                        }
                        if (cam->tipo == folha) { cam->chave[0] = irmaoAnte->chave[irmaoAnte->numChaves-1]; }
                        r->chave[i-1] = irmaoAnte->chave[irmaoAnte->numChaves-1];
                        irmaoAnte->numChaves--;
                        cam->numChaves++;
                    }
                    else if (i < r->numChaves && (r->noInterno.filhos[i+1])->numChaves >= t) {
                        NO* irmaoPos = r->noInterno.filhos[i+1];
                        if (cam->tipo == interno) { 
                            cam->chave[cam->numChaves] = r->chave[i];
                            cam->noInterno.filhos[cam->numChaves+1]= irmaoPos->noInterno.filhos[0];
                        }
                        if (cam->tipo == folha) { cam->chave[cam->numChaves] = irmaoPos->chave[0]; }
                        r->chave[i] = irmaoPos->chave[0];
                        for (int j = 0; j < irmaoPos->numChaves-1; j++) {
                            if (cam->tipo == interno) { irmaoPos->noInterno.filhos[j] = irmaoPos->noInterno.filhos[j+1]; }
                            irmaoPos->chave[j] = irmaoPos->chave[j+1];
                        }
                        irmaoPos->numChaves--;
                        cam->numChaves++;
                    }
                    else if (i > 0) {
                        mergeFilhoArvB(r, r->noInterno.filhos[i-1], cam, i-1);
                        removeArvBmais (r->noInterno.filhos[i-1], k);
                        return;
                    } else {
                        mergeFilhoArvB(r, cam, r->noInterno.filhos[i+1], 0);
                    }
                }
            removeArvBmais (cam, k);
        }
    }
    else if (r->tipo == folha) {
        int j;
        for (j = 0; j < r->numChaves; j++) {
            if (k == r->chave[j]) break;
        }
        while (j < r->numChaves) {
            r->chave[j] = r->chave[j+1];
            j++;
        }
        r->numChaves--;
        return;
    }
}

void removeArvoreB (ArvBmais* T, TipoChave k) {
    if(!T) {
        printf("Erro: nenhuma arvore.");
        return;
    }
    NO* r = T->raiz;
    if (r == NULL || r->numChaves == 0) return;
    else removeArvBmais(r, k);
    if (r->numChaves == 0 && r->tipo == interno) {
        T->raiz = r->noInterno.filhos[0];
        free(r);
    }
}

void imprimeArvoreB (NO* r, FILE* Output) {
    if (r == NULL || r->numChaves == 0) {
        fprintf(Output, "Vazia");
        return;
    }
    if (r->tipo == interno) {
        fprintf(Output, "(");
        int i;
        for (i = 0; i < r->numChaves; i++) {
            imprimeArvoreB(r->noInterno.filhos[i], Output);
            fprintf(Output, " %d ", r->chave[i]);
        }
        imprimeArvoreB(r->noInterno.filhos[r->numChaves], Output);
        fprintf(Output, ")");
    } else {
    fprintf(Output, "(");
    int i;
    for (i = 0; i < r->numChaves; i++) {
        if (i > 0) fprintf(Output," ");
        fprintf(Output, "%d", r->chave[i]);
    }
    fprintf(Output, ")");
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Este programa exige dois argumentos: o arquivo de entrada e o de saida.\n");
        exit(1);
    }
    printf("%s ", argv[0]);
    printf("%s ", argv[1]);
    printf("%s\n", argv[2]);

    ArvBmais baum;
    char filename[MAX_FILENAME] = "";
    strcpy(filename, argv[1]);

    FILE* fp;
    char operacao;
    TipoChave valor;

    fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "ERRO: arquivo NULL.\n");
        return 0;
    }
    criaArvoreB(&baum);

    char saida[MAX_FILENAME] = "";
    strcpy(saida, argv[2]);

    FILE *Output = fopen(saida, "w");
    if (Output == NULL) {
    printf("Erro ao abrir o arquivo de saida.\n");
    return 1;
    }

while (true) {
    fscanf(fp, "%c ", &operacao);
    if (operacao == 'f') {
        fclose(fp);
        return 1;
    }
    else if (operacao == 'i') {
        fscanf(fp, "%d", &valor);
        insereArvoreB(&baum, valor);
    }
    else if (operacao == 'r') {
        fscanf(fp, "%d", &valor);
        removeArvoreB(&baum, valor);
    }
    else if (operacao == 'p') {
        imprimeArvoreB(baum.raiz, Output);
        fprintf(Output, "\n");
    }
}
    fclose(Output);
    return 0;
}
