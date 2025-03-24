/*
============================================================
  Fichero: inout.h
  Creado: 17-03-2025
  Ultima Modificacion: dilluns, 24 de març de 2025, 11:28:59
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include <stdio.h>
#include <stdarg.h>

#define CADLEN 100
#define TABSIZ 4

typedef unsigned char u1;

typedef char Cadena[CADLEN+1];

typedef enum {false,true} bool;

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
