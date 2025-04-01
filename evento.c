/*
============================================================
  Fichero: evento.c
  Creado: 31-03-2025
  Ultima Modificacion: dimarts, 1 d’abril de 2025, 12:22:19
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "evento.h"

#define EVENTOS 256

#define EUS 0 //eventos usar (array)
#define EAN 1 //eventos antes (array)
#define EDS 2 //eventos despues (array)

static Evento eventos[EVENTOS];

static Array aev[3];

static void evinit() {
	Evento* ptr=eventos;
	while(ptr!=eventos+EVENTOS) {
		ptr->id=ptr-eventos;
		ptr->tipo=0;
		ptr->activo=true;
		ptr++;
	}
	for(u1 k=0;k<3;k++) aev[k]=arrnew();
}

static Evento* evfget(u1 id) {
	static bool defined=false;
	if(!defined) {
		evinit();
		defined=true;
	}
	Evento* e=(eventos+id);
	if(e->tipo!=0) e=NULL;
	return e;
}

static void aep(u1 t,u1 id) {
	arrpsh(aev+t,id);
}

static void aee(u1 id) {
	for(u1 k=0;k<3;k++) {
		u1 pos=0;
		if(arrfnd(*(aev+k),id,&pos)) {
			arrera(aev+k,pos);
			break;
		}
	}
}

Evento* evunew(u1 id,u1 o1,u1 o2,Actuacion a) {
	Evento* e=evfget(id);
	if(e) {
		e->tipo=USAR;
		e->objeto[0]=o1;
		e->objeto[1]=o2;
		e->actuacion=a;
		aep(EUS,id);
	}
	return e;
}

static Evento* evnnew(u1 id,u1 t,Actuacion a) {
	Evento* e=evfget(id);
	if(e) {
		e->tipo=t;
		e->actuacion=a;
		aep(t,id);
	}
	return e;
}

Evento* evanew(u1 id,Actuacion a) {
	return evnnew(id,EAN,a);
}

Evento* evdnew(u1 id,Actuacion a) {
	return evnnew(id,EDS,a);
}

void evndel(u1 id) {
	Evento* e=evfget(id);
	if(e->tipo) {
		e->tipo=0;
		aee(id);
	}
}

Evento* eveget(u1 id) {
	Evento* e=eventos+id;
	if(e->tipo) return e;
	else return NULL;
}

bool usachk(u1 o1,u1 o2) {
	u1 ret=false;
	for(u1 k=0;k<aev[EUS].size;k++) {
		u1 id=arrget(aev[EUS],k);
		Evento* e=eveget(id);
		if((e->objeto[0]==o1 && e->objeto[1]==o2) || (e->objeto[0]==o2 && e->objeto[1]==o1)) {
			ret |= e->actuacion(id);
		}
	}
	return ret;
}

bool evachk() {
	bool ret=true;
	for(u1 k=0;k<aev[EAN].size;k++) {
		u1 id=arrget(aev[EAN],k);
		Evento* e=eveget(id);
		ret &= e->actuacion(id);
	}
	return ret;
}
		
void evdchk() {
	for(u1 k=0;k<aev[EDS].size;k++) {
		u1 id=arrget(aev[EDS],k);
		Evento* e=eveget(id);
		e->actuacion(id);
	}
}
	
