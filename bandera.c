/*
============================================================
  Fichero: bandera.c
  Creado: 18-03-2025
  Ultima Modificacion: dimarts, 18 de març de 2025, 11:15:56
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "bandera.h"

#define BANDERAS 256

static bool bandera[BANDERAS];
static bool defined=false;

static void flginit() {
	bool* ptr=bandera;
	while(ptr!=bandera+BANDERAS) {
		*ptr++=false;
	}
}

#define defflg if(!defined && (defined=true)) flginit()

static void flgset(u1 flag,bool value) {
	defflg;
	bandera[flag]=value;
}

void flgon(u1 f) {
	flgset(f,true);
}

void flgoff(u1 f) {
	flgset(f,false);
}

void flgnot(u1 f) {
	defflg;
	bool* pf=bandera+f;
	*pf=!(*pf);
}

bool flgion(u1 f) {
	defflg;
	return bandera[f];
}
