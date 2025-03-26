/*
============================================================
  Fichero: psi.c
  Creado: 18-03-2025
  Ultima Modificacion: dimecres, 26 de març de 2025, 14:30:55
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "psi.h"

static u1 idjugador=0;

bool psinew(u1 i,char* n,char* d,bool j,bool f,u1 a,u1 s,u1 c) {
	if(!j || (j && idjugador==0)) {
		Objeto* opsi=objnew(i,PSI,n);
		if(opsi) {
			cadcpy(opsi->descripcion,d);
			opsi->ataque=a;
			opsi->destreza=s;
			opsi->capacidad=c;
			opsi->jugador=j;
			opsi->amigo=f;
			opsi->vida=MAXVIDA;
			idjugador=(j)?i:idjugador;
			opsi->muerto=false;

			return true;
		}
	}
	return false;
}

u1 psijug() {
	return idjugador;
}

#define isju(A) ((A)==idjugador)

bool psimov(u1 psi,u1 adir) {
	char* salnom[]={"norte","oeste","abajo","arriba","este","sur"};
	Objeto* ojug=objget(idjugador);
	if(ojug) {
		Objeto* opsi=objget(psi);
		u1 locjug=ojug->contenedor;
		if(opsi) {
			u1 oloc=opsi->contenedor;
			if(oloc) {
				u1 dir=adir-1;
				Objeto* ovloc=objget(oloc);
				u1 nloc=ovloc->salida[dir];
				if(nloc!=0) {
					objexp(psi);
					objins(nloc,psi);
					if(isju(psi)) {
						out("Vas %s",salnom[dir]);
					} else if(locjug==oloc) {
						out("%s va hacia %s",opsi->nombre,salnom[dir]);
					} else if(locjug==nloc) {
						out("%s entra desde %s",opsi->nombre,salnom[SALIDAS-1-dir]);
					}
					return true;
				} else if(isju(psi)) {
					out("No hay salida visible en esa direccion...");
				}
			}
		}
	}
	return false;
}

static Objeto* conxnom(Objeto* contenedor,char* nombre) {
	//busca los objetos de un contenedor por nombre
	for(u1 k=0;k<contenedor->contenido.size;k++) {
		u1 nobj=contenedor->contenido.data[k];
		Objeto* obj=objget(nobj);
		if(cadequ(nombre,obj->nombre,true)) {
			return obj;
		}
	}
	return NULL;
}

bool psicog(u1 psi,char* nombre_objeto) {
	char* men[]={	"Lo cojo",
					"No puedo coger nada mas...",
					"No veo eso...",
					"Eso no lo puedo coger..."
	};
	u1 ret=0;
	Objeto* opsi=objget(psi);
	if(opsi) {
		if(opsi->contenido.size<opsi->capacidad) {
			Objeto* localidad=objget(opsi->contenedor);
			Objeto* objeto=conxnom(localidad,nombre_objeto);
			if(objeto) {
				if(objeto->tipo==ITEM && objeto->cogible) {
					objexp(objeto->id);
					objins(psi,objeto->id);
				} else ret=3;
			} else ret=2;
		} else ret=1;
		if(isju(psi)) {
			out("%s",men[ret]);
			outnl(1);
		}
		return (ret==0)?true:false;
	}
	return false;
}

bool psidej(u1 psi,char* nombre_objeto) {
	char* men[]={	"Lo dejo",
					"No lo tengo..."
	};
	u1 ret=0;
	Objeto* opsi=objget(psi);
	Objeto* olocalidad=objget(opsi->contenedor);
	if(opsi && olocalidad) {
		Objeto* objeto=conxnom(opsi,nombre_objeto);
		if(objeto) {
			objexp(objeto->id);
			objins(olocalidad->id,objeto->id);
		} else ret=1;
		if(isju(psi)) {
			out("%s",men[ret]);
			outnl(1);
		}
		return (ret==0)?true:false;
	}
	return false;
}

bool psidsc(u1 psi) {
	char* men[]={	"Descanso, que lo necesito...",
					"No puedo descansar aqui... Hay enemigos... ",
					"A pesar de que no lo necesito, descanso..."
	};
	const u1 APD=1; //aumento por descanso
	u1 ret=0;
	Objeto* opsi=objget(psi);
	Objeto* oloc=(opsi)?objget(opsi->contenedor):NULL;
	if(oloc) {
		bool haypsi=false;
		for(u1 k=0;k<oloc->contenido.size && !haypsi;k++) {
			u1 noc=oloc->contenido.data[k];
			Objeto* oc=objget(noc);
			haypsi=(oc->tipo==PSI) && (oc->amigo!=opsi->amigo);
		}
		if(!haypsi) {
			if(opsi->vida<MAXVIDA) opsi->vida+=APD;	
			else ret=2;
		} else ret=1;
	}
	if(isju(psi)) {
		out("%s",men[ret]);
		outnl(1);
	}
	return (ret!=1);
}

bool psient(u1 psi,char* no) {
	char* men[]={	"Entro...",
					"No veo ese lugar...",
					"No puedo entrar ahi..."
	};
	u1 ret=0;
	Objeto* opsi=objget(psi);
	if(opsi) {
		Objeto* oloc=(opsi)?objget(opsi->contenedor):NULL;
		if(oloc) {
			Objeto* entrada=conxnom(oloc,no);
			if(entrada) {
				if(entrada->tipo==LOCALIDAD) {
					objexp(psi);
					objins(entrada->id,psi);
				} else ret=2;
			} else ret=1;
			if(isju(psi)) {
				out("%s",men[ret]);
				outnl(1);
			}
			return (ret==0)?true:false;
		}
	}
	return false;
}

bool psisal(u1 psi) {
	char* men[]={	"Salgo...",
					"No puedo salir a ningun sitio"
	};
	u1 ret=0;
	Objeto* opsi=objget(psi);
	Objeto* oloc=(opsi)?objget(opsi->contenedor):NULL;
	if(oloc) {
		Objeto* coloc=objget(oloc->contenedor);
		if(coloc && coloc->tipo==LOCALIDAD) {
			objexp(psi);
			objins(coloc->id,psi);
		} else ret=1;
		if(isju(psi)) {
			out("%s",men[ret]);
			outnl(1);
		}
		return (ret==0)?true:false;
	}
	return false;
}

bool psimat(u1 psi) {
	Objeto* opsi=objget(psi);
	if(opsi && opsi->tipo==PSI && !opsi->muerto) {
		u1 nloc=opsi->contenedor;
		opsi->muerto=true;
		opsi->vida=0;
		for(u1 k=0;k<opsi->contenido.size;k++) {
			objins(nloc,opsi->contenido.data[k]);
		}
		if(isju(psi)) {
			out("Has muerto...");
			outnl(1);
			finset(FINLOST);
		} else {
			u1 locpsi=objget(idjugador)->contenedor;
			if(locpsi==nloc) {
				out("%s ha muerto...",opsi->nombre);
				outnl(1);
			}
		}
		return true;
	}
	return false;
}

static u1 atatot(Objeto* psi) {
	u1 ataque=psi->ataque+rnd(1,6);
	u1 arma=0;
	for(u1 k=0;k<psi->contenido.size;k++) {
		Objeto* c=objget(psi->contenido.data[k]);
		if(c->tipo==ITEM && c->plus>arma) arma=c->plus;
	}
	return ataque+arma;
}		

bool psiata(u1 psi,char* np) {
	Objeto* ojug=objget(idjugador);
	Objeto*	jugloc=(ojug)?objget(ojug->contenedor):NULL;
	Objeto* opsi=objget(psi);
	Objeto* psiloc=(opsi)?objget(opsi->contenedor):NULL;
	if(psiloc) {
		Objeto* riv=conxnom(psiloc,np);
		if(riv) {
			if(riv->tipo==PSI) {
				if(ojug==opsi) {
					out("Atacas a %s",riv->nombre);
					outnl(1);
				} else if(riv==ojug) {
					out("%s te ataca",opsi->nombre);
					outnl(1);
				} else if(jugloc==psiloc) {
					out("%s ataca a %s");
					outnl(1);
				}
				u1 difer=atatot(opsi)-atatot(riv);
				if(difer>0) {
					if(jugloc==psiloc) {
						out("El golpe es bueno...");
						outnl(1);
					}
					if(riv->vida<difer) {
						psimat(riv->id);
					} else {
						riv->vida-=difer;
					}
				} else {
					if(jugloc==psiloc) {
						out("El golpe es malo...");
						outnl(1);
					}
				}
				return true;
			} else {
				if(opsi==ojug) {
					out("No puedes atacar a %s...",riv->nombre);
					outnl(1);
				}
			}
		} else {
			if(opsi==ojug) {
				out("No se que es %s...",np);
				outnl(1);
			}
		}
	}
	return false;
}
			
				









			








