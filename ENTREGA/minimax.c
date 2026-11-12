#include"minimax.h"


Node* creaNode(Node* pare, int numFill, int nivell){
    Node* p = malloc(sizeof(Node));
    copiarTauler(pare->tauler, p->tauler);
    tiradaMaq(p->tauler, numFill, nivell);

    int mult = (nivell % 2 == 0)? -1: 1;
    
    if (jugadaGuanyadora(p->tauler, numFill_to_col(pare->tauler, numFill))) {
        p->valor = mult*10000000; // DBL_MAX;
        if (mult > 0) p->valor *= (double) 1/(nivell+1); 
        p->n_fills = 0;
        p->fills = NULL; 
        return p;
    } // SI LA SEGÜENT TIRADA AQUI GUANYA ÉS QUE ENS PODEN TAPAR

    p->valor = 0;
    
    if (nivell < NIVELL_MAX) {
        p->n_fills = calcularNumFills(p->tauler);
        p->fills = malloc(p->n_fills * sizeof(Node*));
    } else {
        p->n_fills = 0;
        p->fills = NULL;
        if (mult == -1) {
            char tConj[M][N];
            copiarTauler(p->tauler, tConj);
            conjugaTauler(tConj);
            p->valor = mult*heuristica(tConj);
        } else p->valor = heuristica(p->tauler);
    }
    return p;
}

void creaFills(Node* pare, int nivell){
    for (int i = 0; i<pare->n_fills; i++) {
        pare->fills[i] = creaNode(pare, i, nivell);
    }
}

void creaArbre(Node* arrel, int nivell){
    creaFills(arrel, nivell);
    
    if (nivell >= NIVELL_MAX) {
        return;
    }

    for (int i = 0; i<arrel->n_fills; i++) {
        creaArbre(arrel->fills[i], nivell+1);
    }
}

void alliberaArbre(Node* arrel) {
    for (int i = 0; i<arrel->n_fills; i++) {
        alliberaArbre(arrel->fills[i]);
    }

    free(arrel->fills);
    free(arrel);
}

void printValuesArbre(Node* arrel, int nivell) {
    if (nivell > 4) return;

    for (int i = 0; i<nivell; i++) {
        printf(" ");
    }
    printf("%f - %d - %d\n", arrel->valor, arrel->n_fills, nivell);
    for (int i = 0; i<arrel->n_fills && i<4; i++) {
        printValuesArbre(arrel->fills[i], nivell+1);
    }
}
void minMax(Node* p, int nivell) {
    
    if (p->n_fills == 0) {
        // if (p->valor == 0.0) p->valor = heuristica(p->tauler);
        return;
    }

    for (int i = 0; i<p->n_fills; i++) {
        minMax(p->fills[i], nivell+1);
    }

    double v = p->fills[0]->valor;
    for (int i = 1; i<p->n_fills; i++) {
        v = (nivell %2 == 0)? max(v, p->fills[i]->valor) : min(v, p->fills[i]->valor);
    }
    p->valor = v;
}

int jugadaMaquina(char tauler[M][N]){
    // clock_t start = clock();
    Node* arrel = malloc(sizeof(Node)); 
    copiarTauler(tauler, arrel->tauler);
    arrel->n_fills = calcularNumFills(tauler); 
    arrel->fills = malloc(arrel->n_fills * sizeof(Node*));
    arrel->valor = 0;
    
    creaArbre(arrel, 1);
    minMax(arrel, 0);
    // printValuesArbre(arrel, 0);
    
    int tirada = 0;
    double v = arrel-> valor;
    for (int i=0; i<arrel->n_fills; i++) {
        if (arrel->fills[i]->valor == v) {
            tirada = numFill_to_col(arrel->tauler, i);
        }
    }
    
    // clock_t end = clock();
    // float seconds = (float)(end - start) / CLOCKS_PER_SEC;
    // printf("La tirada ha durat %f\n", (float)(clock()-start)/CLOCKS_PER_SEC);

    alliberaArbre(arrel);
    return tirada;
}
