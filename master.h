/*
============================================================
  Fichero: master.h
  Creado: 21-03-2025
  Ultima Modificacion: divendres, 21 de març de 2025, 12:31:55
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "jugador.h"

#define TERMW 80 //numero de columnas de la terminal

int trnget();
// da el valor del turno

void trnpss();
// incrementa el turno en 1

void trninc(int i);
// varia el turno en i

void finset(u1 value);
//pone la señal de final con un valor

bool finget(u1* value);
//da true si hay final

void visscr();
// visibiliza la pantalla
