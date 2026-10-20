
#include<stdio.h>
#include<stdlib.h>
#include<time.h> // to make random calls
#include<float.h> // DBL_MAX

typedef struct element {
    int valor;
    struct element* ant;
    struct element* seg;
} Element;

void append(Element* root, int valor){
    while(root->seg != NULL) {
        root = root-> seg;
    }

    Element* nouEl = malloc(sizeof(Element));
    
    nouEl->valor = valor;
    nouEl->ant = root;
    nouEl->seg = NULL;
    root->seg = nouEl;
}

void insert(Element* root, int valor, int pos) {
    for (int i = 0; i<pos && root->seg != NULL; i++) {
        root=root->seg;
    }

    Element* nouEl = malloc(sizeof(Element));
    
    nouEl->valor = valor;
    nouEl->ant = root;
    if (root->seg != NULL) {
        root->seg->ant = nouEl;
        nouEl->seg =  root->seg;
    }
    root->seg = nouEl;
}

void printList(Element* root){
    while(root->seg!= NULL){
        printf("%d, ", root->valor);
        root= root->seg;
    }

    printf("%d", root->valor);
    printf("\n");
}

void alliberaLlista(Element* root){
    while(root->seg!= NULL){
        root= root->seg;
        free(root->ant);
    }

    free(root);
}

int sizeList(Element* root){
    int contador=1;
    while (root->seg!= NULL){
        root= root->seg;
        contador++;
    }
    return contador;
}

Element* creaLlista(int* valors, int n) {
    Element* el = malloc(sizeof(Element));
    el->valor = valors[0];
    el->seg = NULL;
    el->ant = NULL;
    for (int i =1; i<n; i++) {
        append(el, valors[i]);
    }
    return el;
}

void insertAfter(Element* root, int valor) {
    Element* nou = malloc(sizeof(Element));
    nou->valor = valor;

    nou->seg = root->seg;
    nou->seg->ant = nou;
    root->seg = nou;
    nou->ant = root; 
}

void insertBefore(Element* root, int valor) {
    Element* nou = malloc(sizeof(Element));
    nou->valor = valor;

    nou->seg = root;
    nou->ant = root->ant;
    root->ant = nou;
    nou->ant->seg = nou; 
}

int main() { 
    int a[6] = {0, 1, 2, 3, 5, 7};
    Element* b = creaLlista(a, 6); 

    insertAfter(b->seg->seg->seg, 90);
    insertBefore(b->seg->seg, 10);
    printList(b);
    
    alliberaLlista(b);
    
    return 0;
}