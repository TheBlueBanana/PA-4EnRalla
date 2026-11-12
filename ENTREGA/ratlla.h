#include<stdio.h>
#include<stdlib.h>
#include<time.h> // to make random calls
#include<float.h> // DBL_MAX 


#ifndef RATLLA_H
#define RATLLA_H

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

#endif

// UTILITIES
void pause();

double min(double a, double b);
double max(double a, double b);

// TAULER
void printTauler(char tauler[M][N]);
void copiarTauler(char original[M][N], char copia[M][N]);
void buidaTauler(char tauler[M][N]);
void conjugaTauler(char tauler[M][N]);

int numFill_to_col(char tauler[M][N], int numFill);
int calcularNumFills(char tauler[M][N]);
int getTopRow(char tauler[M][N], int col);

double heuristica(char tauler[M][N]);

int tiradaJugador(char tauler[M][N], int col, char player);
void tiradaMaq(char tauler[M][N], int numFill, int nivell);

int maxSeguides(char tauler[M][N], int fila, int col);
int jugadaGuanyadora(char tauler[M][N], int col);

void jugarUnificat(int mode);
void menu();