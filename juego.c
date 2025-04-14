/*
============================================================
  Fichero: juego.c
  Creado: 20-03-2025
  Ultima Modificacion: dilluns, 14 d’abril de 2025, 07:31:01
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "juego.h"

static void mapa_inicia() {
	Laberinto l={	"Mazmorra",
					"Oscura y humeda, huele muy mal...",
					1,3,3,3,1,
					true,true
	};
	Laberinth(l);
	Newline;
}

#define PAN ITMID
#define ESCUDO (PAN+1)
#define CAJA (ESCUDO+1)
#define COFRE (CAJA+1)

static void items_inicia() {
	if(itmnew(CAJA,"Caja","de carton viejo...",true,false,0,0)) {
		objins(rnd(1,27),CAJA);
	}
	if(itmnew(PAN,"Pan","deliciosa y crujiente",true,false,0,0)) {
		objins(CAJA,PAN);
	}
	if(itmnew(ESCUDO,"Escudo","un escudo antiguo y duradero",true,false,5,0)) {
		objins(rnd(1,27),ESCUDO);
	}
	if(itmnew(COFRE,"Cofre","antiguo cofre lleno de monedas",true,false,0,1000)) {
		objins(1,COFRE);
	}
}

#define NUPETSID PSIID
#define NUPETSDIM 20

static void npis_inicia() {
	Raza nuppets={NUPETSID,{NULL,{4,8}},"gomoso y aberrante",false,{1,4},{1,4},{1,4},IAhum,{true}};
	Race(nuppets,NUPETSDIM);
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

	
	
	
