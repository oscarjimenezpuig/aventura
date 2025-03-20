/*
============================================================
  Fichero: juego.c
  Creado: 20-03-2025
  Ultima Modificacion: dijous, 20 de març de 2025, 11:27:26
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
	loccon(3,1,OESTE,true);
}

static void jugador_inicia() {
	jugnew(JUGID);
	objins(JUGID,1);
}

void juego_inicia() {
	mapa_inicia();
	jugador_inicia();
}

int main() {
	juego_inicia();
	return 0;
}

	
	
	
