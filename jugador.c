/*
============================================================
  Fichero: jugador.c
  Creado: 19-03-2025
  Ultima Modificacion: dijous, 20 de març de 2025, 11:31:19
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "jugador.h"

static Objeto* ojug=NULL;
static u1 idjug=0;

typedef struct {
	u1 orden;
	Cadena complemento[2];
} Token;

static void reppoint(u1* p) {
	char* car[]={"ATAQUE","DESTREZA","CAPACIDAD"};
inicio:
	u1 pun=TPI;
	for(u1 k=0;k<4 && pun>0;k++) {
		bool ok=false;
		do {
			out("Puntos disponibles: %i. Puntos dedicados a %s? ",pun,car[k]);
			Cadena s;
			in(s);
			u1 val=0;
			sscanf(s,"%hhi",&val);
			if(val<=pun) {
				ok=true;
				p[k]=val;
				pun-=val;
			}
		} while(!ok);
	}
	out("En resumen: AT:%i HB:%i CP:%i",car[0],car[1],car[2]);
	outnl(1);
	out("Estas de acuerdo? ");
	Cadena r;
	in(r);
	if(*r=='n' || *r=='N') goto inicio;
}

bool jugnew(u1 id) {
	out("Como te llamas? ");
	Cadena s;
	in(s);
	out("Hola %s, vamos a definir tus caracteristicas...");
	outnl(1);
	u1 pun[]={0,0,0};
	reppoint(pun);
	if(psinew(id,s,NULL,true,true,pun[0],pun[1],pun[2])) {
		ojug=objget(id);
		idjug=id;
	}
	return (ojug!=NULL);
}

static u1 separa(Cadena orden,Cadena* frase) {
	char* ptro=orden;
	u1 palabras=0;
	while(*ptro!='\0' && palabras<3) {
		if(cadsep(frase[palabras],&ptro,' ')) palabras++;
	}
	return palabras;
}

static bool actsep(Token tok) {
	switch(tok.orden) {
		case ANORTE:
		case ASUR:
		case AESTE:
		case AOESTE:
		case AARRIBA:
		case AABAJO:
			return psimov(idjug,tok.orden);
		default:
			return false;
	}
}

bool jugact() {
	bool ok=false;
	if(ojug && !ojug->muerto) {
		while(!ok) {
			out("Que quieres hacer? ");
			Cadena orden;
			in(orden);
			Cadena token[3];
			u1 tokens=separa(orden,token);
			if(tokens) {
				Token tok;
				if((tok.orden=accfnd(token[0]))) {
					for(u1 k=0;k<tokens-1;k++) {
						cadcpy(tok.complemento[k],token[k+1]);
					}
					ok=actsep(tok);
				} else {
					out("No entiendo...");
					outnl(1);
				}
			}
		}
	}
	return ok;	
}
