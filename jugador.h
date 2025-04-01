/*
============================================================
  Fichero: jugador.h
  Creado: 19-03-2025
  Ultima Modificacion: dimarts, 1 d’abril de 2025, 08:00:14
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "ia.h"

#define TPI 10 //numero total de puntos a repartir al inicio

bool jugnew(u1 id);
//se define un nuevo jugador

bool jugact();
//se realizan y analizan acciones

bool juglst(u1* orden,char* complemento_1,char* complemento_2);
//obtiene el ultimo token valido introducido por el jugador

