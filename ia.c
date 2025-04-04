/*
============================================================
  Fichero: ia.c
  Creado: 29-03-2025
  Ultima Modificacion: divendres, 4 d’abril de 2025, 11:25:03
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "ia.h"

typedef struct {
	u1 orden;
	Cadena complemento[2];
} Token;

static Token toknew(u1 orden,char* comp1,char* comp2) {
	Token t;
	t.orden=orden;
	cadcpy(t.complemento[0],comp1);
	cadcpy(t.complemento[1],comp2);
	return t;
}

static Token toknul() {
	Token t;
	t.orden=ANULL;
	return t;
}

static Array entradas(Objeto* localidad) {
	Array r=arrnew();
	for(u1 k=0;k<localidad->contenido.size;k++) {
		u1 nobj=arrget(localidad->contenido,k);
		Objeto* onobj=objget(nobj);
		if(onobj->tipo==LOCALIDAD) {
			arrpsh(&r,nobj);
		}
	}
	return r;
}

static bool dasalida(Objeto* psi,Token* tok) {
	Objeto* localidad=objget(psi->contenedor);
	if(localidad) {
		Token token[256];
		u1 tokens=0;
		Objeto* conloc=objget(localidad->contenedor);
		if(conloc && conloc->tipo==LOCALIDAD) {
			token[tokens++]=toknew(ASALIR,"","");
		}
		Array arrentr=entradas(localidad);
		for(u1 k=0;k<arrentr.size;k++) {
			Objeto* ent=objget(arrget(arrentr,k));
			token[tokens++]=toknew(AENTRAR,ent->nombre,"");
		}
		for(u1 k=NORTE;k<=SUR;k++) {
			if(localidad->salida[k]!=0) {
				token[tokens++]=toknew(k+1,"","");
			}
		}
		if(tokens>0) {
			u1 nal=rnd(0,tokens-1);
			*tok=token[nal];
			return true;
		}
	}
	return false;
}

bool iamov(u1 psi) {
	Token t=toknul();
	Objeto* opsi=objget(psi);
	if(dasalida(opsi,&t)) {
		switch(t.orden) {
			case ASALIR:
				return psisal(psi);
			case AENTRAR:
				return psient(psi,t.complemento[0]);
			default:
				return psimov(psi,t.orden);
		}
	}
	return false;
}

static bool dacogible(Objeto* psi,Token* tok) {
	Objeto* loc=objget(psi->contenedor);
	Array cogibles=arrnew();
	if(loc) {
		for(u1 k=0;k<loc->contenido.size;k++) {
			Objeto* ocog=objget(arrget(loc->contenido,k));
			if(ocog && ocog->tipo==ITEM && ocog->cogible) {
				arrpsh(&cogibles,ocog->id);
			}
		}
		if(cogibles.size) {
			u1 nal=rnd(0,cogibles.size-1);
			u1 nobj=arrget(cogibles,nal);
			*tok=toknew(ACOGER,objget(nobj)->nombre,"");
			return true;
		}
	}
	return false;
}

bool iacog(u1 psi) {
	Token t=toknul();
	Objeto* opsi=objget(psi);
	if(opsi->contenido.size<opsi->capacidad && dacogible(opsi,&t)) {
		return psicog(psi,t.complemento[0]);
	}
	return false;
}

static bool dadejable(Objeto* psi,Token* tok) {
	Objeto* dejable=NULL;
	for(u1 k=0;k<psi->contenido.size;k++) {
		Objeto* dej=objget(arrget(psi->contenido,k));
		if(!dejable || dej->plus<dejable->plus) {
			dejable=dej;
		}
	}
	if(dejable) {
		if(psi->contenido.size>1 || dejable->plus==0) {
			*tok=toknew(ADEJAR,dejable->nombre,"");
			return true;
		}
	}
	return false;
}

bool iadej(u1 psi) {
	Token t=toknul();
	Objeto* opsi=objget(psi);
	if(opsi) {
		if(opsi->contenido.size>=opsi->capacidad) {
			if(dadejable(opsi,&t)) {
				return psidej(psi,t.complemento[0]);
			}
		}
	}
	return false;
}

static Array hayene(Objeto* psi) {
	Array ene=arrnew();
	Objeto* oloc=objget(psi->contenedor);
	if(oloc) {
		for(u1 k=0;k<oloc->contenido.size;k++) {
			Objeto* oe=objget(arrget(oloc->contenido,k));
			if(oe && oe->tipo==PSI && oe->amigo!=psi->amigo && !oe->muerto) {
				arrpsh(&ene,oe->id);
			}
		}
	}
	return ene;
}

bool iadsc(u1 psi) {
	Objeto* opsi=objget(psi);
	if(opsi && opsi->vida<MAXFEAT) {
		Objeto* loc=objget(opsi->contenedor);
		if(loc) {
			Array enes=hayene(opsi);
			if(enes.size==0) return psidsc(psi);
		}
	}
	return false;
}

static bool psiatac(Objeto* psi) {
	u1 total=psi->ataque+psi->destreza+(MAXFEAT-(psi->vida));
	u1 nal=rnd(0,total-1);
	return (nal<psi->ataque);
}

bool iaatd(u1 psi) {
	Objeto* opsi=objget(psi);
	if(opsi) {
		Array enes=hayene(opsi);
		if(enes.size) {
			if(psiatac(opsi)) {
				Objeto* oaa=objget(arrget(enes,rnd(0,enes.size-1)));
				return psiata(psi,oaa->nombre);
			} else return psihui(psi);
		}
	}
	return false;
}

bool IAhum(u1 psi) {
	if(!iadsc(psi)) {
		if(!iaatd(psi)) {
			if(!iacog(psi)) {
				if(!iadej(psi)) {
					return iamov(psi);
				}
			}
		}
	}
	return true;
}

bool IAani(u1 psi) {
	if(!iadsc(psi)) {
		if(!iaatd(psi)) {
			return iamov(psi);
		}
	}
	return true;
}

bool IApln(u1 psi) {
	if(!iadsc(psi)) {
		return iaatd(psi);
	}
	return true;
}











	

