// Victor Ruiz Aguilera - 1707258

#include<stdio.h>
#include<stdlib.h>
#include<time.h> // to make random calls
#include<float.h> // DBL_MAX 
#include"ratlla.h"

#ifndef MINIMAX_H
#define MINIMAX_H
    typedef struct node {
        char tauler[M][N];
        struct node** fills; // vector d'apuntadors als fills del node
        int n_fills;
        double valor;
    } Node;
#endif

int jugadaMaquina(char tauler[M][N]);

Node* creaNode(Node* pare, int numFill, int nivell); 
void creaFills(Node* pare, int nivell);
void creaArbre(Node* arrel, int nivell);

void printValuesArbre(Node* arrel, int nivell);
void alliberaArbre(Node* arrel);

void minMax(Node* p, int nivell);