/*
============================================================
  Fichero: master.c
  Creado: 21-03-2025
  Ultima Modificacion: dimecres, 26 de març de 2025, 12:51:39
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

static bool condloc(u1 idobj) {
	Objeto* o=objget(idobj);
	if(o && o->tipo==LOCALIDAD) return true;
	return false;
}

void finfinprt() {
	char* FIN="FIN";
	u1 visitado=0;
	u1 total=0;
	Array localidades=objsel(condloc);
	total=localidades.size;
	for(u1 k=0;k<localidades.size;k++) {
		Objeto* loc=objget(localidades.data[k]);
		if(loc->visitado) visitado++;
	}
	if(total) {
		u1 porcentaje=(visitado*100)/total;
		out("Has descubierto el %i%% del mundo.",porcentaje);
		outnl(1);
	}
	u1 inprt=(TERMW-cadlen(FIN))/2;
	for(u1 k=0;k<inprt;k++) out(" ");
	outat(BOLD);
	out("FIN");
	outnl(1);
	outat(NORMAL);
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






