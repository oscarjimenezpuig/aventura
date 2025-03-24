/*
============================================================
  Fichero: item.c
  Creado: 24-03-2025
  Ultima Modificacion: dilluns, 24 de març de 2025, 11:06:29
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "item.h"

bool itmnew(u1 id,char* n,char* d,bool c,u1 p) {
	Objeto* o;
	if((o=objnew(id,ITEM,n))) {
		if(d) cadcpy(o->descripcion,d);
		o->cogible=c;
		o->plus=p;
		return true;
	}
	return false;
}
