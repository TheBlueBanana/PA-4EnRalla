#include<stdio.h>
#include<stdlib.h>

// ANSI color scape sequences
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define M 6
#define N 8

// this is a test

typedef struct node {
    char** tauler;
    struct node** fills; // vector d'apuntadors als fills del node
    int n_fills;
    double valor;
} Node;

char** crea_tauler();
void allibera_tauler(char** tauler);
void printTauler(char** tauler);
void buida_tauler(char** tauler);
char** copiarTauler(char** original);

int jugadaGuanyadora(char** tauler, int col);

int min(int a, int b) {
    return (a<b)? a : b;
}
int max(int a, int b) {
    return (a>b)? a : b;
}

void printTauler(char** tauler){
    for (int i = M-1; i>=0; i--) {
        printf("%d ", i+1);
        for (int j = 0; j<N; j++) {
            if (tauler[i][j] == 'x') printf(ANSI_COLOR_BLUE);
            else if (tauler[i][j] == 'o') printf(ANSI_COLOR_RED);

            printf("%c " ANSI_COLOR_RESET, tauler[i][j]);
        }
        printf("\n");
    }
    for (int i = 0; i<=N; i++){
        printf("%d ", i);
    }
    printf("\n");
}

char** copiarTauler(char** original){
    char** copia = crea_tauler();
    for (int i = 0; i<M; i++) {
        for (int j = 0; j<N; j++){
            copia[i][j] = original[i][j];
        }
    }
    return copia;
}

/* UNUSED
    retorna la posició buida més baixa:
    0: cap peça posada
    M: fila plena
*/
int getTopRow(char** tauler, int col){
    int fila = 0;
    for (; fila < M; fila++)
    {
        if (tauler[fila][col] == '-'){
            break;
        }
    }
    return fila;
}

/* Fa una tirada al tauler del jugador seleccionat. Si la tirada és invàlida retorna 0 */
int tiradaJugador(char** tauler, int col, char player) {
    if (col < 0 || col>= N)
        return 0;

    for (int i = 0; i < M; i++)
    {
        if (tauler[i][col] == '-'){
            tauler[i][col] = player;
            return 1;
        }
    }
    return 0;
}

/* Comproba si la darrera jugada guanya la partida 
    1 - guanya
    0 - no guanya
    -1 - error
*/
int jugadaGuanyadora(char** tauler, int col) {
    if (col < 0 || col>= N)
        return 0;

    int fila = M-1;
    char player = 'e';
    for (; fila >= 0; fila--)
    {
        if (tauler[fila][col] != '-'){
            player = tauler[fila][col];
            break;
        }
    }

    if (player == 'e') return -1;

    // Horitzontal
    int seguides = 1;
    for (int i = 1; i<= N-col; i++) {
        if (tauler[fila][col+i] != player) break;
        seguides++;
    }
    for (int i = 1; i<= col; i++) {
        if (tauler[fila][col-i] != player) break;
        seguides++;
    }

    if (seguides >= 4) return 1;

    // Vertical
    seguides = 1;
    for (int i = 1; i <= fila; i++) {
        if (tauler[fila-i][col] != player) break;
        seguides++;
    }

    if (seguides >= 4) return 1;
    
    /* Obliqua \ */
    seguides = 1;
    for (int i = 1; i<= min(N-col, fila); i++) {
        if (tauler[fila-i][col+i] != player) break;
        seguides++;
    }
    for (int i = 1; i<= min(col, M-fila-1); i++) {
        if (tauler[fila+i][col-i] != player) break;
        seguides++;
    }

    if (seguides >= 4) return 1;
    
    // Obliqua /
    seguides = 1;
    for (int i = 1; i<= min(N-col, M-fila-1); i++) {
        if (tauler[fila+i][col+i] != player) break;
        seguides++;
    }
    for (int i = 1; i<= min(col, fila); i++) {
        if (tauler[fila-i][col-i] != player) break;
        seguides++;
    }

    if (seguides >= 4) return 1;

    return 0;
}

#pragma region Nodes i tal
// void tirada(char tauler[N][N], int numFill) {
// 	//1.Transformar el numFill a columna de la matriu.
//     int columna = 0;
// 	//2.Calcula la fila on cau per gravetat
// 	//3.Posar la fitxa a la fila/columna calculada
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

#pragma endregion

char** crea_tauler() {
    char** tauler = (char**) malloc(M*sizeof(char*));
    if (tauler == NULL) {
        printf("MEMORY ERROR!");
        return NULL;
    }
    for (int i = 0; i<M; i++) {
        tauler[i] = (char*) malloc(N*sizeof(char));

        if (tauler[i] == NULL) {
            printf("MEMORY ERROR!");
            allibera_tauler(tauler);
            return NULL;
        }
    }
    return tauler;
}

void allibera_tauler(char** tauler){
    for (int i = 0; i<M; i++) {
        free(tauler[i]); // Alliberem cada fila
    }
    free(tauler); // Alliberem el tauler
}

void buida_tauler(char** tauler) {
    for (int i = 0; i<M; i++) {
        for (int j = 0; j<N; j++) {
            tauler[i][j] = '-';
        }
    }
}

void jugarPersones() {
    char** tauler = crea_tauler();
    buida_tauler(tauler);
    printTauler(tauler);

    char player = 'x';
    for (int i = 0; i<N*M; i++) {
        player = (i%2)? 'o' : 'x';
        printf("torn del jugador %c. La seva jugada:\n", player);
        int play = 0;
        scanf("%d", &play);
        play--;
        if (tiradaJugador(tauler, play, player) == 0) 
            printf("Error tirant!\n");

        if (jugadaGuanyadora(tauler, play) == 1) {
            printTauler(tauler);
            printf("guanya %c!\n", player);
            break;
        }
        printTauler(tauler);
    }
    
    printf("Entra qualsevol número per tornar al menú\n");
    
    int a; // CANVIAR!!
    scanf("%d", &a);
    
    allibera_tauler(tauler);
}

void jugarMaquina() {
    char** tauler = crea_tauler();
    buida_tauler(tauler);
    printTauler(tauler);

    char player = 'x';
    for (int i = 0; i<N*M; i++) {
        player = (i%2)? 'o' : 'x';
        printf("torn del jugador %c. La seva jugada:\n", player);
        int play = 0;
        scanf("%d", &play);
        play--;
        if (tiradaJugador(tauler, play, player) == 0) 
            printf("Error tirant!\n");

        if (jugadaGuanyadora(tauler, play) == 1) {
            printTauler(tauler);
            printf("guanya %c!\n", player);
            break;
        }
        printTauler(tauler);
    }
    
    printf("Entra qualsevol número per tornar al menú\n");
    
    int a; // CANVIAR!!
    scanf("%d", &a);
    
    allibera_tauler(tauler);
}

void menu() {
    int option = 0;
    do
    {
        printf("Què vols fer?\n");
        printf("1. Jugar sol\n");
        printf("2. Jugar contra la màquina\n");
        printf("3. Sortir\n");
        scanf("%d", &option);

        switch (option)
        {
            case 1:
                jugarPersones();
                break;
            case 2:
                jugarMaquina();
                break;

            case 0:
            case -1:
            case 3:
            case 4:
            case 5:
            default:
                option = 0;
                break;
        }

    } while (option != 0);
    
}

int main(){
    
    printf("Benvinguda!\n");
    menu();

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

