/*
============================================================
  Fichero: juego.c
  Creado: 20-03-2025
  Ultima Modificacion: divendres, 4 d’abril de 2025, 11:18:06
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

#define PAN ITMID
#define ESCUDO (PAN+1)
#define CAJA (ESCUDO+1)

static void items_inicia() {
	if(itmnew(CAJA,"Caja","de carton viejo...",true,false,0)) {
		objins(3,CAJA);
	}
	if(itmnew(PAN,"Pan","deliciosa y crujiente",true,false,0)) {
		objins(CAJA,PAN);
	}
	itmnew(ESCUDO,"Escudo","un escudo antiguo y duradero",true,false,5);
}

#define TALON PSIID
#define OGRO (TALON+1)

static void npis_inicia() {
	psinew(TALON,"Talon","guapo y bello",false,true,5,3,1,IAhum);
	objins(2,TALON);
	objins(TALON,ESCUDO);
	psinew(OGRO,"Ogro","muy feo y con mocos",false,false,7,7,3,IAhum);
	objins(5,OGRO);
}

#define USAPAN 0

static bool usapan(u1 idevento) {
	out("Me como el pan... Esta bueno... ");
	outnl(1);
	objexp(PAN);
	eveget(USAPAN)->activo=false;
	return true;
}

static void usar_inicia() {
	evunew(USAPAN,PAN,0,usapan);
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
	usar_inicia();
	return jugador_inicia();
}

int main() {
	if(juego_inicia()) {
		u1 valuefin=0;
		while(!finget(&valuefin)) {
			visscr();
			jugact();
			pssact();
			trnpss();
		}
		finfinprt();
	}
	return 0;
}

	
	
	
