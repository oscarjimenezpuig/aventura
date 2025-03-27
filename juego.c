/*
============================================================
  Fichero: juego.c
  Creado: 20-03-2025
  Ultima Modificacion: dijous, 27 de març de 2025, 11:08:11
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
	locnew(4,"Caverna","Una gran caverna llena de estalagtitas");
	locnew(5,"Pasillo","Un pequeño pasillo muy oscuro");
	objins(3,4);
	loccon(4,5,SUR,true);
}

static void items_inicia() {
	u1 itid=ITMID;
	if(itmnew(itid,"Pan","Una bonita barra de pan",true,0)) {
		objins(3,itid++);
	}
}

static void npis_inicia() {
	static u1 npis=PSIID;
	psinew(npis,"Talon","Guapo y bello",false,true,5,3,1);
	objins(5,npis);
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
	npis_inicia();
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
		finfinprt();
	}
	return 0;
}

	
	
	
