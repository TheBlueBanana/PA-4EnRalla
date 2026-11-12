#include"ratlla.h"
#include"minimax.c"

#pragma region Utilities

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
        player = (torn == 0)? MAQUINA_CHAR : PLAYER_CHAR;
    }
    
    for (int i = 0; i<N*M; i++) {
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
                play = jugadaMaquina(tauler);
            }
        }
        
        if (tiradaJugador(tauler, play, player) == 0) {
            printf("Error tirant! Torna a provar siusplau.\n");
            i--;
            printTauler(tauler);
            continue;
        }
        
        printf("Tauler actual:\n");
        printTauler(tauler);
        if (jugadaGuanyadora(tauler, play) == 1) {
            printf("guanya %c!\n", player);
            break;
        } else if (i>=N*M-1) { // ACABAR
            printf("Empat!\n");
            break;
        }

        if (mode == 2) {
            printf("Entra qualsevol número per continuar\n");
            pause();
        }
        
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