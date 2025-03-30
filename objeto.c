/*
============================================================
  Fichero: objeto.c
  Creado: 16-03-2025
  Ultima Modificacion: diumenge, 30 de març de 2025, 11:08:55
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
	Array ene=arrnew();
	Array ami=arrnew();
	Array otr=arrnew();
	for(u1 k=0;k<o->contenido.size;k++) {
		Objeto* oc=objget(arrget(o->contenido,k));
		if(oc->tipo==PSI && oc->jugador) continue;
		if(oc->tipo==PSI && !oc->muerto) {
			if(oc->amigo) arrpsh(&ami,oc->id);
			else arrpsh(&ene,oc->id);
		} else {
			arrpsh(&otr,oc->id);
		}
	}
	if(ene.size) {
		out("Atencion, enemigos en esta localidad...");
		outnl(1);
		outtb(1);
		for(u1 k=0;k<ene.size;k++) {
			Objeto* oene=objget(arrget(ene,k));
			if(oene) {
				out("-%s",oene->nombre);
				outnl(1);
			}
		}
	}
	if(ami.size) {
		out("Tienes amigos aqui...");
		outnl(1);
		outtb(1);
		for(u1 k=0;k<ami.size;k++) {
			Objeto* oene=objget(arrget(ami,k));
			if(oene) {
				out("-%s",oene->nombre);
				outnl(1);
			}
		}
	}
	if(!ene.size && (otr.size || conloc)) {
		out("Aqui puedes ver: ");
		outnl(1);
		if(conloc) {
			outtb(1);
			out("-Salida a %s",conloc->nombre);
		}
		for(u1 k=0;k<otr.size;k++) {
			outtb(1);
			Objeto* oc=objget(arrget(otr,k));
			if(oc->tipo==LOCALIDAD) {
				out("-Entrada a ");
			} else if(oc->tipo==PSI) {
				out("-Cadaver de ");
			} else out("-");
			out("%s",oc->nombre);
			outnl(1);
		}
	}
}

static u1 getstr(u1 car) {
	const u1 DSC=4;
	const u1 MED=MAXFEAT/DSC;
	u1 ret=0;
	u1 ini=MAXFEAT;
	while(ini<=MAXFEAT) {
		if(car>=ini-MED) return ret;
		ret++;
		ini-=MED;
	}
	return (DSC-1);
}

static void psiprt(Objeto* o) {
	char* ataque[]={	"muy fuerte",
						"fuerte",
						"debil",
						"muy debil"
	};
	char* salud[]={		"muy buena",
						"buena",
						"mala",
						"muy mala"
	};
	char* habilidad[]={	"muy habil",
						"habil",
						"torpe",
						"muy torpe"
	};
	if(!o->jugador && !o->muerto) {
		out("%s",o->nombre);
		if(cadlen(o->descripcion)) out(", %s. ",o->descripcion);
		else out(". ");
		out("Es %s, %s y tiene %s salud. ",ataque[getstr(o->ataque)],habilidad[getstr(o->destreza)],salud[getstr(o->vida)]);
		if(o->contenido.size>0) {
			out("Va cargando alguna cosa...");
		}
		outnl(1);
	} else if(o->muerto) {
		out("El cadaver de %s, que en paz descanse...",o->nombre);
		outnl(1);
	}
}

static void itemprt(Objeto* o) {
	out("%s",o->nombre);
	if(cadlen(o->descripcion)) {
		out(", %s. ",o->descripcion);
	} else {
		out(", no tiene nada especial. ");
	}
	if(o->plus) {
		out("Es un arma (+%hhu).",o->plus);
	}
	outnl(1);
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
			case PSI:
				return psiprt(o);
			case ITEM:
				return itemprt(o);
			default:
				return defprt(o);
		}
	}
}


