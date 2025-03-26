/*
============================================================
  Fichero: master.h
  Creado: 21-03-2025
  Ultima Modificacion: dimecres, 26 de març de 2025, 13:59:14
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "inout.h"

#define TERMW 80 //numero de columnas de la terminal

#define FINQUIT 0 //final cuando se conecta quit
#define FINLOST 1 //final si jugador muere

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

void finfinprt();
//da el final total con el tanto por ciento de mapa visitado y la palabra fin

void visscr();
// visibiliza la pantalla
