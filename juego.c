/*
============================================================
  Fichero: juego.c
  Creado: 20-03-2025
  Ultima Modificacion: dilluns, 24 de març de 2025, 11:05:59
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "juego.h"

static void mapa_inicia() {
	locnew(1,"Prado","Un bonito y verde prado.");
	locnew(2,"Riachuelo","Un riachuelo que circula entre arbustos.");
	locnew(3,"Bosque","Un bosque con cuatro pinos");
	loccon(1,2,NORTE,true);
	loccon(2,3,ESTE,true);
	loccon(3,1,SUR,false);
}

static void items_inicia() {
	u1 itid=ITMID;
	if(itmnew(itid,"Pan","Una bonita barra de pan",true,0)) {
		objins(3,itid++);
	}
}

static bool jugador_inicia() {
	if(jugnew(JUGID)) objins(1,JUGID);
	else {
		out("ERROR en definicion de jugador");
		outnl(1);
		return false;
	}
	return true;
}

bool juego_inicia() {
	mapa_inicia();
	items_inicia();
	return jugador_inicia();
}

int main() {
	if(juego_inicia()) {
		u1 valuefin=0;
		while(!finget(&valuefin)) {
			visscr();
			jugact();
			trnpss();
		}
	}
	return 0;
}

	
	
	
