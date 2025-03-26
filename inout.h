/*
============================================================
  Fichero: inout.h
  Creado: 17-03-2025
  Ultima Modificacion: dimecres, 26 de març de 2025, 14:16:18
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include <stdio.h>
#include <stdarg.h>

#include "rand.h"

#define CADLEN 100
#define TABSIZ 4


typedef char Cadena[CADLEN+1];

typedef enum {NORMAL=0,BOLD=1,INVERSE=7} Atributo;

char* in(Cadena cadena);

void out(const char* cadena,...);

void outnl(u1 lines);
/* nueva linea */

void outtb(u1 tab);
/* tabulacion, a base de espacios */

void outat(Atributo a);
/* attributos de escritura */

void cadcpy(Cadena d,char* o);
/* copia un string en una cadena */

bool cadequ(char* a,char* b,bool insensitive);
/* mira si dos cadenas son iguales insensitive=true: case insensitive*/

bool cadsep(Cadena d,char** f,char separador);
/* separa f y obtiene la primera palabra diferente al vacio, responde true si encuentra palabra*/

u1 cadlen(char* c);
/* da la longitud de la cadena */
