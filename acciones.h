/*
============================================================
  Fichero: acciones.h
  Creado: 17-03-2025
  Ultima Modificacion: dissabte, 22 de març de 2025, 20:57:33
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "mapa.h"

#define SINS 10 /* numero de sinonimos maximo */

#define ANULL 0
#define ANORTE 1
#define AOESTE 2
#define AABAJO 3
#define AARRIBA 4
#define AESTE 5
#define ASUR 6
#define ACOGER 7
#define ADEJAR 8
#define AENTRAR 9
#define ASALIR 10
#define AUSAR 11
#define AATACAR 12
#define AHUIR 13
#define ADESCANSAR 14
#define AFINALIZAR 15

#define ACCIONES AFINALIZAR+1

u1 accfnd(char* cadena);
/* busca una accion del deposito de acciones */




