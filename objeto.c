/*
============================================================
  Fichero: objeto.c
  Creado: 16-03-2025
  Ultima Modificacion: diumenge, 16 de març de 2025, 13:22:00
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "objeto.h"

#define OBJETOS 256

static Objeto objetos[OBJETOS];

static void obsini() {
	Objeto* ptr=objetos;
	while(ptr!=objetos+OBJETOS) {
		ptr->tipo=NIL;
		ptr++;
	}
}

Objeto* objnew(u1 i,u1 t,char* n) {
	static bool init=false;
	Objeto* o=NULL;
	if(t!=NIL && i>0) {
		if(!init && (init=true)) obsini();
		o=objetos+i;
		o->tipo=t;
		o->nombre=n;
	}
	return o;
}

Objeto* objget(u1 i) {
	static bool init=false;
	if(!init && (init=true)) obsini();
	Objeto* o=objetos+i;
	return (i==0 || o->tipo==NIL)?NULL:o;
}

Array objsel(Condicion c) {
	Array a=arrnew();
	Objeto* ptr=objetos;
	while(ptr!=objetos+OBJETOS) {
		if(ptr->tipo!=NIL && c(ptr)) arrpsh(&a,ptr->id);
		ptr++;
	}
	return a;
}

	
	
