/*
============================================================
  Fichero: item.c
  Creado: 24-03-2025
  Ultima Modificacion: dimecres, 2 d’abril de 2025, 13:30:08
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "item.h"

bool itmnew(u1 id,char* n,char* d,bool c,bool cr,u1 p) {
	Objeto* o;
	if((o=objnew(id,ITEM,n))) {
		if(d) cadcpy(o->descripcion,d);
		o->cogible=c;
		o->plus=p;
		o->cerrada=cr;
		return true;
	}
	return false;
}
