/*
============================================================
  Fichero: inout.h
  Creado: 17-03-2025
  Ultima Modificacion: dilluns, 17 de març de 2025, 11:05:19
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include <stdio.h>
#include <stdarg.h>

#define CADLEN 20
#define TABSIZ 4

typedef unsigned char u1;

typedef char Cadena[CADLEN+1];

typedef enum {false,true} bool;

char* in(Cadena cadena);

void out(const char* cadena,...);

void outnl(u1 lines);
/* nueva linea */

void outtb(u1 tab);
/* tabulacion, a base de espacios */

void cadcpy(Cadena d,char* o);
/* copia un string en una cadena */

bool cadequ(char* a,char* b);
/* mira si dos cadenas son iguales */




