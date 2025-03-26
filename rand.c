/*
============================================================
  Fichero: rand.c
  Creado: 26-03-2025
  Ultima Modificacion: dimecres, 26 de març de 2025, 14:31:59
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "rand.h"

int rnd(int a,int b) {
	static bool defined=false;
	if(!defined) {
		srand(time(NULL));
		defined=true;
	}
	int min=(a>b)?b:a;
	int max=(a>b)?a:b;
	int dif=max-min+1;
	return min+(rand()%dif);
}
