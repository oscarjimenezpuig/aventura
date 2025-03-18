/*
============================================================
  Fichero: objeto.c
  Creado: 16-03-2025
  Ultima Modificacion: dimarts, 18 de març de 2025, 12:28:55
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
		o->descripcion=NULL;
		o->contenedor=0;
		o->contenido=arrnew();
	}
	return o;
}

Objeto* objget(u1 i) {
	static bool init=false;
	if(!init && (init=true)) obsini();
	Objeto* o=objetos+i;
	return (i==0 || o->tipo==NIL)?NULL:o;
}

bool objins(u1 r,u1 o) {
	Objeto* oo=objget(o);
	if(oo && oo->contenedor==0) {
		Objeto* or=objget(r);
		if(or && arrpsh(&(or->contenido),o)) {
			oo->contenedor=r;
			return true;
		}
	}
	return false;
}

bool objexp(u1 o) {
	Objeto* oo=objget(o);
	u1 r;
	if((r=oo->contenedor)) {
		Objeto* or=objget(r);
		u1 pos;
		arrfnd(or->contenido,o,&pos);
		arrera(&(or->contenido),pos);
		oo->contenedor=0;
		return true;
	}
	return false;
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

static void locprt(Objeto* o) {
	char* NOMSAL[]={"NORTE","OESTE","ABAJO","ARRIBA","ESTE","SUR"};
	out(o->descripcion);
	out("Salidas visibles: ");
	u1 hs=0;
	for(u1 s=0;s<SALIDAS;s++) {
		if(o->salida[s] && (hs=1)) out("%s ",NOMSAL[s]);
	}
	if(!hs) {
		out("No hay");
	}
	outnl(1);
	if(o->contenido.size>0) {
		out("Aqui puedes ver: ");
		for(u1 k=0;k<o->contenido.size;k++) {
			outtb(1);
			out("-%s",o->nombre);
			outnl(1);
		}
	}
}

static void defprt(Objeto* o) {
	out("%s, %s...",o->nombre,o->descripcion);
	outnl(1);
}

void objprt(u1 id) {
	Objeto* o=objget(id);
	if(o) {
		switch(o->tipo) {
			case LOCALIDAD:
				return locprt(o);
			default:
				return defprt(o);
		}
	}
}

	
	
