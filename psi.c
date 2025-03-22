/*
============================================================
  Fichero: psi.c
  Creado: 18-03-2025
  Ultima Modificacion: dissabte, 22 de març de 2025, 20:59:47
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "psi.h"

static u1 idjugador=0;

bool psinew(u1 i,char* n,char* d,bool j,bool f,u1 a,u1 s,u1 c) {
	if(!j || (j && idjugador==0)) {
		Objeto* opsi=objnew(i,PSI,n);
		if(opsi) {
			cadcpy(opsi->descripcion,d);
			opsi->ataque=a;
			opsi->destreza=s;
			opsi->capacidad=c;
			opsi->jugador=j;
			opsi->amigo=f;
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

#define isju(A) ((A)==idjugador)

bool psimov(u1 psi,u1 adir) {
	char* salnom[]={"norte","oeste","abajo","arriba","este","sur"};
	Objeto* ojug=objget(idjugador);
	if(ojug) {
		Objeto* opsi=objget(psi);
		u1 locjug=ojug->contenedor;
		if(opsi) {
			u1 oloc=opsi->contenedor;
			if(oloc) {
				u1 dir=adir-1;
				Objeto* ovloc=objget(oloc);
				u1 nloc=ovloc->salida[dir];
				if(nloc!=0) {
					objexp(psi);
					objins(nloc,psi);
					if(isju(psi)) {
						out("Vas %s",salnom[dir]);
					} else if(locjug==oloc) {
						out("%s va hacia %s",opsi->nombre,salnom[dir]);
					} else if(locjug==nloc) {
						out("%s entra desde %s",opsi->nombre,salnom[SALIDAS-1-dir]);
					}
					return true;
				} else if(isju(psi)) {
					out("No hay salida visible en esa direccion...");
				}
			}
		}
	}
	return false;
}

					



