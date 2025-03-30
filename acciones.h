/*
============================================================
  Fichero: acciones.h
  Creado: 17-03-2025
  Ultima Modificacion: diumenge, 30 de març de 2025, 09:11:44
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
#define AINVENTARIO 15
#define ANADA 16
#define AEXAMINAR 17
#define AFINALIZAR 18

#define ACCIONES AFINALIZAR+1

u1 accfnd(char* cadena);
/* busca una accion del deposito de acciones */




