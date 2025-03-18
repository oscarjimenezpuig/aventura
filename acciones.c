/*
============================================================
  Fichero: acciones.c
  Creado: 17-03-2025
  Ultima Modificacion: dilluns, 17 de març de 2025, 11:18:35
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "acciones.h"

typedef struct {
	u1 id;
	u1 sinonimos;
	Cadena verbo[SINS];
} Accion;

Accion acciones[ACCIONES];

static void accnew(u1 id,char* verbo) {
	Accion* a=acciones+id;
	a->id=id;
	a->sinonimos=1;
	cadcpy(a->verbo[0],verbo);
}

static void accins(u1 id,char* verbo) {
	Accion* a=acciones+id;
	if(a->sinonimos<SINS) cadcpy(a->verbo[a->sinonimos++],verbo);
}

static void accdef() {
	accnew(1,"norte");
	accnew(2,"oeste");
	accnew(3,"baja");
	accnew(4,"sube");
	accnew(5,"este");
	accnew(6,"sur");
	accnew(7,"coger");
	accnew(8,"dejar");
	accnew(9,"entrar");
	accnew(10,"salir");
	accnew(11,"usar");
	accnew(12,"atacar");
	accnew(13,"huir");
	accnew(14,"nada");
	accins(14,"descansa");
}

u1 accfnd(char* a) {
	static bool defined=false;
	if(!defined && (defined=true)) accdef();
	for(u1 id=1;id<ACCIONES;id++) {
		Accion* ac=acciones+id;
		for(u1 s=0;s<ac->sinonimos;s++) {
			if(cadequ(a,ac->verbo[s])) return id;
		}
	}
	return ANULL;
}





