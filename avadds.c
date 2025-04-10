/*
============================================================
  Fichero: avadds.c
  Creado: 10-04-2025
  Ultima Modificacion: dijous, 10 d’abril de 2025, 11:44:40
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "avadds.h"

#define DN 1
#define DS 2
#define DE 4
#define DW 8

static Laberinto actual;

typedef struct {
	u1 salidas;
	u1 camino;
} Habitacion;

static Habitacion* lab=NULL;

static bool labnew() {
	int size=actual.w*actual.h;
	lab=malloc(sizeof(Habitacion)*size);
	if(lab) {
		int pos=rnd(0,size-1);
		Habitacion* ptr=lab;
		while(ptr!=lab+size) {
			*ptr=(Habitacion){0,0};
			if(ptr-lab==pos) ptr->camino=1;
			ptr++;
		}
		return true;
	}
	return false;
}

static void labdel() {
	free(lab);
	lab=NULL;
}

static Habitacion* romget(u1 x,u1 y) {
	if(x<actual.w && y<actual.h) {
		return lab+(x+y*actual.w);
	}
	return NULL;
}

static void labcon(u1 x,u1 y) {
	const u1 SAL[2][2]={{DN,DS},{DE,DW}};
	static u1 camino=2;
	Habitacion* a=romget(x,y);
	a->camino=camino;
random:
	u1 sx=x;
	u1 sy=y;
	int avc=rnd(0,1);
	int dir=rnd(0,1);
	if(dir) sx+=(avc)?1:-1;
	else sy+=(avc)?1:-1;
	Habitacion* b=romget(sx,sy);
	if(b) {
		bool cont=(b->camino==0 || b->camino==camino);
		a->salidas |= SAL[dir][avc];
		if(cont) {
			labcon(sx,sy);
		} else {
			camino++;
		}
	} else goto random;
}

static bool labtry() {
	bool res=false;
	for(u1 x=0;x<actual.w;x++) {
		for(u1 y=0;y<actual.h;y++) {
			Habitacion* r=romget(x,y);
			if(r->camino==0) {
				labcon(x,y);
				res=true;
			}
		}
	}
	return res;
}

static void labprt() {
	for(u1 y=0;y<actual.h;y++) {
		for(u1 x=0;x<actual.w;x++) {
			Habitacion* r=romget(x,y);
			printf("%02i ",r->salidas);
		}
		printf("\n");
	}
}

static labtoplan(u1 level,u1 x,u1 y) {
	u1 id=actual.idini+x+y*actual.w + l*actual.w*actual.h;
	


static void plantabuilt(u1 level) {
	labnew();
	while(labtry());
	
	labdel();
}
			
/* prueba */

int main() {
	actual.w=4;
	actual.h=3;
	planta();
	return 0;
}









