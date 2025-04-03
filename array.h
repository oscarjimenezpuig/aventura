/*
============================================================
  Fichero: array.h
  Creado: 16-03-2025
  Ultima Modificacion: dijous, 3 d’abril de 2025, 11:05:54
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "bandera.h"

#define ARRSIZE 256

typedef struct {
	u1 size;
	u1 data[ARRSIZE];
} Array;

Array arrnew();

u1 arrget(Array array,u1 pos);

void arrclr(Array* array);

bool arrpsh(Array* array,u1 value);

bool arrins(Array* array,u1 size,u1* values);

bool arrfnd(Array array,u1 value,u1* pos);

void arrera(Array* array,u1 pos);

void arrprt(Array array);





