/*
============================================================
  Fichero: psi.c
  Creado: 18-03-2025
  Ultima Modificacion: dimarts, 18 de març de 2025, 12:27:39
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "psi.h"

static u1 idjugador=0;

bool psinew(u1 i,char* n,char* d,bool j,u1 a,u1 s,u1 c) {
	if(!j || (j && idjugador==0)) {
		Objeto* opsi=objnew(i,PSI,n);
		if(opsi) {
			opsi->descripcion=d;
			opsi->ataque=a;
			opsi->destreza=s;
			opsi->capacidad=c;
			opsi->jugador=j;
			idjugador=(j)?i:idjugador;
			opsi->muerto=false;

			return true;
		}
	}
	return false;
}

u1 psijug() {
	return idjugador;
}

u1 psipos(u1 p,u1 l) {
	Objeto* ol=objget(l);
	Objeto* op=objget(p);
	if(!op->contenedor) {
		

#define isju(A) ((A)==idjugador)

bool psimov(u1 psi,u1 dir) {
	Objeto* loc=objget(objget(psi)->contenedor);

	



