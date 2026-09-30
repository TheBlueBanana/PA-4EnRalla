#include<stdio.h>
#include<stdlib.h>

#define N 8
#define M 8

// this is a test

typedef struct node {
    char tauler[N][M];
    struct node** fills; // vector d'apuntadors als fills del node
    int n_fills;
    double valor;
} Node;

void allibera_tauler(char** tauler);
char** crea_tauler();

void printTauler(char** tauler){
    for (int i = 0; i<N; i++) {
        for (int j = 0; j<M; j++) {
            printf("%c", tauler[i][j]);
        }
        printf("\n");
    }
}

void copiarTauler(char tauler1[N][M], char tauler2[N][M]){

}

// void tirada() {
//     // 1 calcular a quina columna correspon la tirada (i-essima col lliure =/= i-essima col)

// }

// int calcularNumFills(char tauler[N][M], int numFill) {
    
//     return 0;
// }

// Node* creaNode(Node* pare, int numFill, int nivell){
//     Node* p = malloc(sizeof(Node));
//     copiarTauler(p->tauler, pare->tauler);
//     tirada(p->tauler, numFill);
//     if (nivell < 2) {
//         p->n_fills = calcularNumFills(p->tauler); 
//         p->fills = malloc(p->n_fills * sizeof(Node*));
//     } else {
//         p->n_fills = NULL;
//     }
//     return p;
// }

// void creaFills(Node* pare, int nivell){
//     for (int i = 0; i<pare->n_fills; i++) {
//         pare->fills[i] = creaNode(pare, i, nivell);
//     }
// }

// void creaArbre(Node* arrel){
//     creaFills(arrel);
//     for (int i = 0; i<arrel->n_fills; i++) {
//         creaFills(arrel->fills[i], 2);
//     }
// }


char** crea_tauler() {
    char** tauler = (char**) calloc(N, sizeof(char*));
    if (tauler == NULL) {
        printf("MEMORY ERROR!");
        return NULL;
    }
    for (int i = 0; i<N; i++) {
        tauler[i] = (char*) calloc(M, sizeof(char));

        if (tauler[i] == NULL) {
            printf("MEMORY ERROR!");
            allibera_tauler(tauler);
            return NULL;
        }
    }
    return tauler;
}

void allibera_tauler(char** tauler){
    for (int i = 0; i<N; i++) {
        free(tauler[i]); // Alliberem cada fila
    }
    free(tauler); // Alliberem el tauler
}


int main(){
    printf("%d\n",N);
    printf("%d\n",M);

    char** tauler = crea_tauler(); // = (char**) calloc(N*M, sizeof(char));

    printTauler(tauler);
    for (int i = 0; i<N; i++) {
        for (int j = 0; j<M; j++) {
            tauler[i][j] = '-';
        }
        printf("\n");
    }
    tauler[0][1] = 'x';
    printf("%c\n", tauler[0][1]);
    printTauler(tauler);
    allibera_tauler(tauler);

    // Node arrel;
    // arrel.n_fills = N;
    // arrel.fills = malloc(N*sizeof(Node*));
    // creaArbre(arrel);

    return 0;
}


// QUICKREF

// per llegir números
    // int n =0;
    // scanf("%d", &n);
    // printf("has dit %d\n", n);

