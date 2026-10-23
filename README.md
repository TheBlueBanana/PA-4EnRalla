# PA-4EnRalla
Primer treball de PA

EXAM:
es pot portar tot
~4 exercicis
tot en pc


CPP:
si posem un & a la declaració de la funció, aquesta es passará com a referència, no per valor
void func(Complex& c1) { } // es passa per referència sense importar que l'usuari no canvii res!

ORDRES DE PRECOMPILACIÓ:
#ifndef __f__
#define __f__
...
#endif
 // Això és per definir estructures es creen etiquetes de forma que sabem si s'ha definit ja i no tenim duplicacions!

OBSERVACIONS:

compilar: gcc -Wall fitxer.c -o sortida
executar: ./sortida

capçeleres: #include "capçelera.h"
// amb <-> busca el fitxer a la root de C i amb "-" el busca a la pròpia carpeta

variable global d'un altre fitxer
extern double VARIABLE;

compilació:
-Wall //all warings
-O1 -O2 -O3 //augmenten velocitats 
-std="VERSION" //canvia la versió del compilador
