/*
============================================================
  Fichero: objeto.c
  Creado: 16-03-2025
  Ultima Modificacion: dimecres, 26 de març de 2025, 12:46:51
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "objeto.h"

#define OBJETOS 256

static Objeto objetos[OBJETOS];

static void obsini() {
	static bool init=false;
	if(!init) {
		Objeto* ptr=objetos;
		while(ptr!=objetos+OBJETOS) {
			ptr->tipo=NIL;
			ptr++;
		}
		init=true;
	}
}

Objeto* objnew(u1 i,u1 t,char* n) {
	obsini();
	Objeto* o=NULL;
	if(t!=NIL && i>0) {
		o=objetos+i;
		o->id=i;
		o->tipo=t;
		cadcpy(o->nombre,n);
		o->contenedor=0;
		o->contenido=arrnew();
	}
	return o;
}

Objeto* objget(u1 i) {
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
	int id=1;
	while(id<OBJETOS) {
		if(c(id)) arrpsh(&a,id);
		id++;
	}
	return a;
}

static void locprt(Objeto* o) {
	char* NOMSAL[]={"NORTE","OESTE","ABAJO","ARRIBA","ESTE","SUR"};
	out(o->descripcion);
	outnl(1);
	out("Salidas visibles: ");
	u1 hs=0;
	for(u1 s=0;s<SALIDAS;s++) {
		if(o->salida[s] && (hs=1)) out("%s ",NOMSAL[s]);
	}
	if(!hs) {
		out("No hay");
	}
	outnl(1);
	Objeto* conloc=objget(o->contenedor);
	conloc=(conloc && conloc->tipo==LOCALIDAD)?conloc:NULL;
	if(o->contenido.size>1 || conloc) {
		out("Aqui puedes ver: ");
		outnl(1);
		if(conloc) {
			outtb(1);
			out("-Salida a %s",conloc->nombre);
		}
		for(u1 k=0;k<o->contenido.size;k++) {
			outtb(1);
			Objeto* oc=objget(o->contenido.data[k]);
			if(oc->tipo==PSI && oc->jugador) continue;
			if(oc->tipo==LOCALIDAD) {
				out("-Entrada a ");
			} else out("-");
			out("%s",oc->nombre);
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


