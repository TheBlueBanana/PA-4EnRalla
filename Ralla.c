// V - 13/10/26

#include<stdio.h>
#include<stdlib.h>
#include<time.h> // to make random calls
// #include<ncurses.h> // to getch

// #ifdef OSisWindows
//     #include<conio.h>
//     void 
// #else

// #endif

// distingir sistemes operatius
// https://stackoverflow.com/questions/142508/how-do-i-check-os-with-a-preprocessor-directive


// ANSI color scape sequences
// https://stackoverflow.com/questions/3219393/stdlib-and-colored-output-in-c
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"


#define M 6
#define N 8
#define NIVELL_MAX 4

#define PLAYER_CHAR 'x'
#define MAQUINA_CHAR 'o'
#define EMPTY_CHAR '-'

// this is a test

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

#pragma region Utilities
/*  MATH */
int min(int a, int b) {
    return (a<b)? a : b;
}
int max(int a, int b) {
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

/* UNUSED
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

    // for (int i = 0; i < M; i++)
    // {
    //     if (tauler[i][col] == EMPTY_CHAR){
    //         tauler[i][col] = player;
    //         return 1;
    //     }
    // }
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

    int fila = M-1;
    char player = 'e';
    for (; fila >= 0; fila--)
    {
        if (tauler[fila][col] != EMPTY_CHAR){
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
#pragma endregion

#pragma region Nodes i tal
void tiradaMaq(char tauler[M][N], int numFill) {
	//1.Transformar el numFill a columna de la matriu.
    int col = numFill_to_col(tauler, numFill);
    
	//2.Calcula la fila on cau per gravetat
    tauler[getTopRow(tauler, col)][col] = MAQUINA_CHAR;
    
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
        printf("ERROR: Columna Out Of Range a numFill_to_col");
        return -1;
    } else if (tauler[M-1][col] != EMPTY_CHAR) {
        printf("ERROR: Columna Plena a numFill_to_col");
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
    tiradaMaq(p->tauler, numFill);
    
    if (nivell < NIVELL_MAX) {
        p->n_fills = calcularNumFills(p->tauler); 
        p->fills = malloc(p->n_fills * sizeof(Node*));
    } else {
        p->n_fills = 0;
        p->fills=NULL;
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
    
    if (nivell +1 >= NIVELL_MAX) {
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
    for (int i = 0; i<nivell; i++) {
        printf(" ");
    }
    printf("%f - %d\n", arrel->valor, arrel->n_fills);
    for (int i = 0; i<arrel->n_fills; i++) {
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
    double v = 0;
    if (tauler[0][N-1] != EMPTY_CHAR) v++;
    if (tauler[0][3] != PLAYER_CHAR) v++;
    if (tauler[0][4] != PLAYER_CHAR) v--;
    v = ((double)rand())/RAND_MAX;
    return v;
}

void recorreArbreRecursiu(Node* p, int nivell) {
    
    if (p->n_fills == 0) {
        p->valor = heuristica(p->tauler);
        return;
    }

    for (int i = 0; i<p->n_fills; i++) {
        recorreArbreRecursiu(p->fills[i], nivell+1);
    }

    double v = p->fills[0]->valor;
    for (int i =1; i<p->n_fills; i++) {
        v = (nivell %2 ==1)? min(v, p->fills[i]->valor) : max(v, p->fills[i]->valor);
    }
    p->valor = v;
}


int jugadaMaquina(char tauler[M][N]){
    Node* arrel = malloc(sizeof(Node)); // AARREGLAR!!!!!!!!!!!
    copiarTauler(tauler, arrel->tauler);
    creaArbre(arrel, 1);
    recorreArbreRecursiu(arrel, 1);
    printValuesArbre(arrel, 0);
    
    int tirada = 0;
    double v = arrel-> valor;
    for (int i=0; i<arrel->n_fills; i++) {
        if (arrel->fills[i]->valor == v) {
            tirada = numFill_to_col(arrel->tauler, i);
        }
    }

    alliberaArbre(arrel);
    return tirada;
}

#pragma endregion

#pragma region Loops de Joc
void jugarPersones() {
    char tauler[M][N]; // = creaTauler();
    buidaTauler(tauler);
    printTauler(tauler);
    
    char player = PLAYER_CHAR;
    for (int i = 0; i<N*M; i++) {
        player = (i%2)? MAQUINA_CHAR : PLAYER_CHAR;
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
    // getch();
    
    // alliberaTauler(tauler);
}

void jugarMaquina() {
    char tauler[M][N]; // = creaTauler();
    buidaTauler(tauler);
    printTauler(tauler);
    
    
    char player = PLAYER_CHAR;
    int torn = 1; // 0 màquina; 1 humà
    
    // Per començar
    // printf("Vols començar? (0:no, 1:si)");
    // scanf("%d", &torn);
    torn = torn % 2;
    
    for (int i = 0; i<N*M+100; i++) {
        int play = 0;
        if (torn == 1) {
            printf("torn del jugador %c. La seva jugada:\n", player);
            scanf("%d", &play);
            play--;
        } else {

            
            play = jugadaMaquina(tauler);//rand() % N; // queda mirar que sigui vàlida
        }
        
        if (tiradaJugador(tauler, play, player) == 0) {
            printf("Error tirant! Torna a provar siusplau.\n");
            printTauler(tauler);
            continue;
        }
        
        if (jugadaGuanyadora(tauler, play) == 1) {
            printTauler(tauler);
            printf("guanya %c!\n", player);
            break;
        } else if (i>=N*M) { // ACABAR
            printf("Empat!\n");
            break;
        }
        printTauler(tauler);
        
        if (torn == 0) {
            torn = 1;
            player = PLAYER_CHAR;
        } else {
            torn = 0;
            player = MAQUINA_CHAR;
        }
        
    }
    
    printf("Entra qualsevol número per tornar al menú\n");
    
    int a; // CANVIAR!!
    scanf("%d", &a);
    
    // alliberaTauler(tauler);
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
#pragma endregion

int main(){
    srand(time(NULL));
    
    printf("Benvinguda!\n");
    menu();

    // Node arrel;
	// arrel.valor=0;  //valor posat pel recorreArbre
	// arrel.n_fills=N;
    // arrel.tauler = creaTauler();
    // buidaTauler(arrel.tauler);
	// arrel.fills=malloc(N*sizeof(Node *));
    // printf("Hi\n");
	// creaArbre(&arrel);
    // recorreArbreRecursiu(&arrel,0);
    return 0;

    // Node arrel;
    // arrel.n_fills = N;
    // arrel.fills = malloc(N*sizeof(Node*));
    // creaArbre(&arrel);
    // recorreArbre(&arrel);

    return 0;
}


// QUICKREF

// per llegir números
    // int n =0;
    // scanf("%d", &n);
    // printf("has dit %d\n", n);

