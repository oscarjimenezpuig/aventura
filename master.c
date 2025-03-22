/*
============================================================
  Fichero: master.c
  Creado: 21-03-2025
  Ultima Modificacion: dissabte, 22 de març de 2025, 20:54:48
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "jugador.h"

static int turno=1;
static u1 finval=0;
static bool finis=false;

int trnget() {
	return turno;
}

void trnpss() {
	++turno;
}

void trninc(int i) {
	turno+=i;
}

void finset(u1 v) {
	finval=v;
	finis=true;
}

bool finget(u1* v) {
	if(finis) *v=finval;
	return finis;
}

void visscr() {
	Objeto* jug=objget(psijug());
	u1 njugloc=0;
	if(!jug->muerto && (njugloc=jug->contenedor)) {
		Objeto* jugloc=objget(njugloc);
		outat(INVERSE);
		out("%-70s TURNO %03i",jugloc->nombre,turno);
		outat(NORMAL);
		outnl(1);
		objprt(njugloc);
		outnl(1);
	}
}






