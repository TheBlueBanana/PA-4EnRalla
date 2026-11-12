// V - 12/11/26 

#include<stdio.h>
#include<stdlib.h>
#include<time.h> // to make random calls
#include<float.h> // DBL_MAX
// #include<ncurses.h> // to getch


// ANSI color scape sequences
// https://stackoverflow.com/questions/3219393/stdlib-and-colored-output-in-c
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"


#define M 8
#define N 8
#define NIVELL_MAX 6

#define PLAYER_CHAR 'x'
#define MAQUINA_CHAR 'o'
#define EMPTY_CHAR '-'

// distingir sistemes operatius
// https://stackoverflow.com/questions/142508/how-do-i-check-os-with-a-preprocessor-directive
void pause(){
    #ifdef WIN32
    // #include <conio.h>

        getchar();
    
    #elif defined(__linux__)
        int a; // CANVIAR!!
        scanf("%d", &a);
        // #include <whatever the curses header is>
    #else
        int a; // CANVIAR!!
        scanf("%d", &a);
        // #include <whatever the curses header is>
    #endif
}
        
typedef struct node {
    char tauler[M][N];
    struct node** fills; // vector d'apuntadors als fills del node
    int n_fills;
    double valor;
} Node;

void printTauler(char tauler[M][N]);
void buidaTauler(char tauler[M][N]);
void copiarTauler(char original[M][N], char copia[M][N]);

int jugadaGuanyadora(char tauler[M][N], int col);

int numFill_to_col(char tauler[M][N], int numFill);
double heuristica(char tauler[M][N]);
int maxSeguides(char tauler[M][N], int fila, int col);

#pragma region Utilities
/*  MATH */
double min(double a, double b) {
    return (a<b)? a : b;
}
double max(double a, double b) {
    return (a>b)? a : b;
}

#pragma endregion

#pragma region Gestió de Tauler
/* BASICS */
void printTauler(char tauler[M][N]){
    for (int i = M-1; i>=0; i--) {
        printf("%d ", i+1);
        for (int j = 0; j<N; j++) {
            if (tauler[i][j] == PLAYER_CHAR) printf(ANSI_COLOR_BLUE);
            else if (tauler[i][j] == MAQUINA_CHAR) printf(ANSI_COLOR_RED);

            printf("%c " ANSI_COLOR_RESET, tauler[i][j]);
        }
        printf("\n");
    }
    for (int i = 0; i<=N; i++){
        printf("%d ", i);
    }
    printf("\n");
}

void copiarTauler(char original[M][N], char copia[M][N]){
    for (int i = 0; i<M; i++) {
        for (int j = 0; j<N; j++){
            copia[i][j] = original[i][j];
        }
    }
}

void buidaTauler(char tauler[M][N]) {
    for (int i = 0; i<M; i++) {
        for (int j = 0; j<N; j++) {
            tauler[i][j] = EMPTY_CHAR;
        }
    }
}

void conjugaTauler(char tauler[M][N]) {
    for (int i = 0; i<M; i++) {
        for (int j = 0; j<N; j++) {
            if (tauler[i][j] == PLAYER_CHAR) {
                tauler[i][j] = MAQUINA_CHAR;
            } else if (tauler[i][j] == MAQUINA_CHAR) {
                tauler[i][j] = PLAYER_CHAR;
            }
            
        }
    }
}

/* 
    retorna la posició buida més baixa:
    0: cap peça posada
    M: fila plena
*/
int getTopRow(char tauler[M][N], int col){
    int fila = 0;
    for (; fila < M; fila++)
    {
        if (tauler[fila][col] == EMPTY_CHAR){
            break;
        }
    }
    return fila;
}

/* GAME */

/* Fa una tirada al tauler del jugador seleccionat. Si la tirada és invàlida retorna 0 */
int tiradaJugador(char tauler[M][N], int col, char player) {
    if (col < 0 || col>= N)
        return 0;

    int row = getTopRow(tauler, col);
    if(row < M) {
        tauler[row][col] = player;
        return 1;
    }

    return 0;
}

/* Comproba si la darrera jugada guanya la partida 
    1 - guanya
    0 - no guanya
    -1 - error
*/
int jugadaGuanyadora(char tauler[M][N], int col) {
    if (col < 0 || col>= N)
        return 0;

    int fila = getTopRow(tauler, col);
    if (fila == 0) return -1;
    fila--;
    // char player = tauler[--fila][col]; 

    if (maxSeguides(tauler, fila, col) == 4) return 1;
    return 0;
}

/*
    1-4: nombre màxim de peçes seguides
    5: 3 seguides i extensible!
*/
int maxSeguides(char tauler[M][N], int fila, int col) {

    char player = tauler[fila][col];

    int maxSeg = 1;
    // int extensible = 0;
    // Horitzontal
    int seguides = 1;
    for (int i = 1; i< N-col; i++) {
        if (tauler[fila][col+i] != player) {
            // if (tauler[fila][col+i] == EMPTY_CHAR) extensible = 1;
            break;
        }
        seguides++;
    }
    for (int i = 1; i<= col; i++) {
        if (tauler[fila][col-i] != player) {
            // if (tauler[fila][col-i] == EMPTY_CHAR) extensible = 1;
            break;
        }
        seguides++;
    }

    if (seguides >= 4) return 4;
    // if (seguides == 3 && extensible) return 5;
    // extensible = 0;
    maxSeg = max(seguides, maxSeg);

    // Vertical
    seguides = 1;
    for (int i = 1; i <= fila; i++) {
        if (tauler[fila-i][col] != player) {
            // if (tauler[fila-i][col] == EMPTY_CHAR) extensible = 1;
            break;
        }
        seguides++;
    }
    for (int i = 1; i<= M-fila-1; i++) {
        if (tauler[fila+i][col] != player) {
            // if (tauler[fila+i][col] == EMPTY_CHAR) extensible = 1;        
            break;
        }
        seguides++;
    }

    if (seguides >= 4) return 4;
    // if (seguides == 3 && extensible) return 5;
    // extensible = 0;
    maxSeg = max(seguides, maxSeg);

    
    /* Obliqua \ */
    seguides = 1;
    for (int i = 1; i<= min(N-col-1, fila); i++) {
        if (tauler[fila-i][col+i] != player) {
            // if (tauler[fila-i][col+i] == EMPTY_CHAR) extensible = 1;    
            break;
        }
        seguides++;
    }
    for (int i = 1; i<= min(col, M-fila-1); i++) {
        if (tauler[fila+i][col-i] != player) {
            // if (tauler[fila+i][col-i] == EMPTY_CHAR) extensible = 1;    
            break;
        }
        seguides++;
    }

    if (seguides >= 4) return 4;
    // if (seguides == 3 && extensible) return 5;
    // extensible = 0;
    maxSeg = max(seguides, maxSeg);

    // Obliqua /
    seguides = 1;
    for (int i = 1; i<= min(N-col-1, M-fila-1); i++) {
        if (tauler[fila+i][col+i] != player) {
            // if (tauler[fila+i][col+i] == EMPTY_CHAR) extensible = 1;    
            break;
        }
        seguides++;
    }
    for (int i = 1; i<= min(col, fila); i++) {
        if (tauler[fila-i][col-i] != player) {
            // if (tauler[fila-i][col-i] == EMPTY_CHAR) extensible = 1;    
            break;
        }
        seguides++;
    }

    if (seguides >= 4) return 4;
    // if (seguides == 3 && extensible) return 5;
    return  max(seguides, maxSeg);
}
#pragma endregion

#pragma region Nodes i tal
void tiradaMaq(char tauler[M][N], int numFill, int nivell) {
	//1.Transformar el numFill a columna de la matriu.
    int col = numFill_to_col(tauler, numFill);
    
	//2.Calcula la fila on cau per gravetat
    tauler[getTopRow(tauler, col)][col] = (nivell % 2 == 0)? PLAYER_CHAR: MAQUINA_CHAR;
    
	//3.Posar la fitxa a la fila/columna calculada
}

int numFill_to_col(char tauler[M][N], int numFill){
    int col = numFill;
    for (int i = 0; i<=col && i<N; i++) {
        if (tauler[M-1][i] != EMPTY_CHAR){
            col++;
        }
    }
    if (col >= N) {
        printf("ERROR: Columna Out Of Range a numFill_to_col\n");
        return -1;
    } else if (tauler[M-1][col] != EMPTY_CHAR) {
        printf("ERROR: Columna Plena a numFill_to_col\n");
        return -1;
    }
    return col;
}

int calcularNumFills(char tauler[M][N]) {
    int possibles = 0;
    for (int i = 0; i<N; i++) {
        if (tauler[M-1][i] == EMPTY_CHAR) possibles++;
    }
    return possibles;
}

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

/*
void recorreArbreRecursiu(Node* p, int nivell) {
    for (int k = 0; k<nivell;k++) {
        printf("  ");
    }
    printf("%.0f\n", p->valor);
    for (int i =0; i<p->n_fills; i++) {
        recorreArbreRecursiu(p->fills[i], nivell+1);
    }
    } 
    void recorreArbre(Node* arrel) {
        for(int i=0;i<arrel->n_fills;i++) {
            printf("%.0f\n",arrel->fills[i]->valor);
            for(int j=0;j<arrel->fills[i]->n_fills;j++) {
                printf("  %.0f\n",arrel->fills[i]->fills[j]->valor);
            }
        }
    }
*/
    
double heuristica(char tauler[M][N]) {
    // calculem lo bo per jugador
    // calculem lo bo per maquina
    // fem avg

    // IDEA: Iterar només sobre els buits i veure si podem extendre allà!!!
    double v = 0; 
    char tAux[M][N];
    copiarTauler(tauler, tAux);
    int altures[N+2];
    for (int i = 1; i<=N; i++) {
        altures[i] = getTopRow(tauler, i);
    }
    altures[0] = altures[1];
    altures[N+1] = altures[N];
    // int maxHeight = 0;
    // for (int i = 0; i<N; i++) {
    //     maxHeight = max(getTopRow(tauler, i), maxHeight);
    // }

    for (int i =0; i<M; i++) {
        for (int j = 0; j<N; j++) {
            if (tauler[i][j] != EMPTY_CHAR) continue;
            // if (j > maxHeight) continue;
            if(j> max(altures[j], max(altures[j+1], altures[j+2]))) continue;

            tAux[i][j] = PLAYER_CHAR;
            if (maxSeguides(tAux, i, j) == 4) v += -10;

            tAux[i][j] = MAQUINA_CHAR;
            if (maxSeguides(tAux, i, j) == 4) v += 10;

            tAux[i][j] = EMPTY_CHAR;
        }
    }
    
    return v + ((double)rand())/RAND_MAX;
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

#pragma endregion

#pragma region Loops de Joc

/*
    Mode:   0 - 2 jugadors
            1 - 1 jugador vs maquina
            2 - maquina sola
*/
void jugarUnificat(int mode) {
    
    char tauler[M][N]; // = creaTauler();
    buidaTauler(tauler);
    printTauler(tauler);
    
    
    char player = PLAYER_CHAR;
    int torn = 1; // 0 màquina; 1 humà
    if (mode == 1) {
        // Per començar
        printf("Vols començar? (0:no, 1:si)\n");
        scanf("%d", &torn);
        torn = torn % 2;
    }
    
    for (int i = 0; i<=N*M; i++) {
        int play = 0;
        if (torn == 1) {
            if (mode <= 1) {
                printf("torn del jugador %c. La seva jugada:\n", player);
                scanf("%d", &play);
                play--;
            } else {
                char tConj[M][N];
                copiarTauler(tauler, tConj);
                conjugaTauler(tConj);
                play = jugadaMaquina(tConj);
            }
        } else {
            if (mode == 0) {
                printf("torn del jugador %c. La seva jugada:\n", player);
                scanf("%d", &play);
                play--;
            } else {
                play = jugadaMaquina(tauler);//rand() % N; // queda mirar que sigui vàlida
            }
        }
        
        if (tiradaJugador(tauler, play, player) == 0) {
            printf("Error tirant! Torna a provar siusplau.\n");
            i--;
            printTauler(tauler);
            continue;
        }
        
        if (jugadaGuanyadora(tauler, play) == 1) {
            printTauler(tauler);
            printf("guanya %c!\n", player);
            break;
        } else if (i==N*M) { // ACABAR
            printf("Empat!\n");
            break;
        }
        printTauler(tauler);

        printf("Entra qualsevol número per continuar\n");
        pause();
        
        if (torn == 0) {
            torn = 1;
            player = PLAYER_CHAR;
        } else {
            torn = 0;
            player = MAQUINA_CHAR;
        }
    }
    
    printf("Entra qualsevol número per tornar al menú\n");
    
    pause();
}

void menu() {
    int option = 0; 
    do
    {
        printf("Què vols fer?\n");
        printf("1. Jugar sol\n");
        printf("2. Jugar contra la màquina\n");
        printf("3. Que jugui ella sola\n");
        printf("4. Sortir\n");
        scanf("%d", &option);
        
        switch (option)
        {
            case 1:
            jugarUnificat(0);
            // jugarPersones();
            break;
            case 2:
            jugarUnificat(1);
            // jugarMaquina();
            break;
            
            case 3:
            jugarUnificat(2);
            // jugaMaquinaSola();
            break;

            default:
            option = 0;
            break;
        }
        
    } while (option != 0);
    
}
#pragma endregion

int main(){
    srand(time(NULL));
    // srand(123);
    
    printf("Benvinguda!\n");
    menu();

    return 0;
}