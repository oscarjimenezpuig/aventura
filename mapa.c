/*
============================================================
  Fichero: mapa.c
  Creado: 16-03-2025
  Ultima Modificacion: dimecres, 26 de març de 2025, 11:18:30
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "mapa.h"

bool locnew(u1 i,char* n,char* d) {
	Objeto* o=objnew(i,LOCALIDAD,n);
	if(o) {
		cadcpy(o->descripcion,d);
		for(u1 k=0;k<SALIDAS;k++) o->salida[k]=0;
		o->visitado=false;
	}
	return (o==NULL)?false:true;
}

bool loccon(u1 o,u1 d,u1 s,bool b) {
	Objeto* oo=objget(o);
	Objeto* od=objget(d);
	if(oo && od && oo->tipo==LOCALIDAD && od->tipo==LOCALIDAD) {
		oo->salida[s]=d;
		if(b) od->salida[SALIDAS-s-1]=o;
		return true;
	}
	return false;
}
		

		
