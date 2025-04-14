/*
============================================================
  Fichero: avadds.c
  Creado: 10-04-2025
  Ultima Modificacion: dilluns, 14 d’abril de 2025, 05:58:04
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

/*
static void labprt() {
	for(u1 y=0;y<actual.h;y++) {
		for(u1 x=0;x<actual.w;x++) {
			Habitacion* r=romget(x,y);
			printf("%02i ",r->salidas);
		}
		printf("\n");
	}
}
*/

static u1 getid(u1 level,u1 x,u1 y) {
	return actual.idini+x+y*actual.w + level*actual.w*actual.h;
}

static void move(u1 dir,int x,int y,int* dx,int* dy) {
	switch(dir) {
		case DN:
			--y;
			break;
		case DS:
			++y;
			break;
		case DE:
			--x;
			break;
		case DW:
			++x;
			break;
	}
	*dx=x;
	*dy=y;
}

static u1 altsal(u1 sal) {
	switch(sal) {
		case DN:
			return NORTE;
		case DS:
			return SUR;
		case DE:
			return ESTE;
		default:
			return OESTE;
	}
}

static void labtoplan(u1 level,u1 x,u1 y) {
	Habitacion* r=romget(x,y);
	int dx,dy;
	for(u1 dir=DN;dir<=DW;dir=dir<<1) {
		if(r->salidas & dir) {
			move(dir,x,y,&dx,&dy);
			loccon(getid(level,x,y),getid(level,dx,dy),altsal(dir),true);
		}
	}
}

static void plantabuilt(u1 level) {
	labnew();
	while(labtry());
	for(u1 i=0;i<actual.w;i++) {
		for(u1 j=0;j<actual.h;j++) {
			labtoplan(level,i,j);
		}
	}
	labdel();
}

static void plantasbuilt() {
	for(u1 k=0;k<actual.altura;k++) plantabuilt(k);
}

static void plantajoin(u1 level) {
	for(u1 k=0;k<actual.escaleras;k++) {
		Objeto* loin=NULL;
		Objeto* lofi=NULL;
		u1 x,y,id;
		while(!loin) {
			x=rnd(0,actual.w-1);
			y=rnd(0,actual.h-1);
			id=getid(level,x,y);
			loin=objget(id);
			if(!loin || loin->salida[ARRIBA]!=0) loin=NULL;
		}
		while(!lofi) {
			x=rnd(0,actual.w-1);
			y=rnd(0,actual.h-1);
			id=getid(level+1,x,y);
			lofi=objget(id);
			if(!lofi || lofi->salida[ABAJO]!=0) lofi=NULL;
		}
		loccon(loin->id,lofi->id,ARRIBA,true);
	}
}

static void plantasjoin() {
	for(u1 l=0;l<actual.altura-1;l++) {
		plantajoin(l);
	}
}

static void habitdef() {
	u1 number=1;
	for(u1 l=0;l<actual.altura;l++) {
		for(u1 i=0;i<actual.w;i++) {
			for(u1 j=0;j<actual.h;j++) {
				u1 id=getid(l,i,j);
				locnew(id,actual.nombre,actual.descripcion);
				Objeto* oloc=objget(id);
				if(actual.numnom) {
					char snumero[30];
					sprintf(snumero," %i",number);
					cadadd(oloc->nombre,snumero);
				}
				if(actual.levdes) {
					char snivel[30];
					sprintf(snivel,"\nEstas en el piso %i.",l+1);
					cadadd(oloc->descripcion,snivel);
				}
				++number;
			}
		}
	}
}

void labset(Laberinto l) {
	actual=l;
	habitdef();
	plantasbuilt();
	plantasjoin();
}

static u1 rndpos(Zona zona) {
	static bool todosdef=false;
	static Array todos;
	if(zona.todo && !todosdef) {
		todos=arrnew();
		for(int k=0;k<256;k++) {
			Objeto* o=objget(k);
			if(o && o->tipo==LOCALIDAD) arrpsh(&todos,k);
		}
		todosdef=true;
	}
	Array obj=(zona.todo)?todos:zona.localidades;
	if(obj.size) return arrget(obj,rnd(0,obj.size-1));
	else return 0;
}

#define inrnd(I) rnd((I).min,(I).max)
#define tocap(A) ((A)-'a'+'A')

static bool rndlet(bool vocal) {
	const u1 VOCS=6;
	char* VOCA="aeiouy";
	char FRST='b';
	char LAST='z';
	if(vocal) {
		return VOCA[rnd(0,VOCS-1)];
	} else {
		char plet=0;
		while(!plet) {
			plet=rnd(FRST,LAST);
			char* pv=VOCA;
			while(*pv!='\0' && plet) {
				if(*pv==plet) plet=0;
				pv++;
			}
		}
		return plet;
	}
}

static void rndname(Cadena nombre,u1 len) {
	len=(len==0)?1:len;
	bool tipo=rnd(0,1);
	u1 k=0;
	for(;k<len && k<CADLEN;k++) {
		if(k==0) *nombre=tocap(rndlet(tipo));
		else nombre[k]=rndlet(tipo);
		tipo=!tipo;
	}
	nombre[k]='\0';
}



static bool onemember(u1 id,Raza r) {
	Cadena nombre;
	if(r.nombre.nombre) cadcpy(nombre,r.nombre.nombre);
	else rndname(nombre,inrnd(r.nombre.letras));
	if(psinew(id,nombre,r.descripcion,false,r.amigo,inrnd(r.ataque),inrnd(r.destreza),inrnd(r.capacidad),r.ia)) {
		u1 pos=rndpos(r.zona);
		if(pos) return objins(pos,id);
	}
	return false;
}


u1 raznew(Raza r,u1 m) {
	u1 id=r.idini;
	bool create=false;
	do {
		create=onemember(id,r);
	} while (create && id-r.idini<m && (++id));
	return id-r.idini;
}






