/*
============================================================
  Fichero: array.h
  Creado: 16-03-2025
  Ultima Modificacion: diumenge, 16 de març de 2025, 13:22:00
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include <stdio.h>

#define ARRSIZE 256

typedef enum {false,true} bool;

typedef unsigned char u1;

typedef struct {
	u1 size;
	u1 data[ARRSIZE];
} Array;

Array arrnew();

void arrclr(Array* array);

bool arrpsh(Array* array,u1 value);

bool arrins(Array* array,u1 size,u1* values);

bool arrfnd(Array array,u1 value,u1* pos);

void arrera(Array* array,u1 pos);

void arrprt(Array array);





