/*
============================================================
  Fichero: psi.c
  Creado: 18-03-2025
  Ultima Modificacion: divendres, 4 d’abril de 2025, 11:08:44
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "psi.h"

static u1 idjugador=0;

bool psinew(u1 i,char* n,char* d,bool j,bool f,u1 a,u1 s,u1 c,IA ia) {
	if(!j || (j && idjugador==0)) {
		Objeto* opsi=objnew(i,PSI,n);
		if(opsi) {
			cadcpy(opsi->descripcion,d);
			opsi->ataque=(a>MAXFEAT)?MAXFEAT:a;
			opsi->destreza=(s>MAXFEAT)?MAXFEAT:s;
			opsi->capacidad=c;
			opsi->jugador=j;
			opsi->amigo=f;
			opsi->vida=MAXFEAT;
			idjugador=(j)?i:idjugador;
			opsi->muerto=false;
			opsi->ia=ia;
			return true;
		}
	}
	return false;
}

u1 psijug() {
	return idjugador;
}

static void objvis(Array* destino,u1 nobjeto) {
	arrpsh(destino,nobjeto);
	Objeto* obj=objget(nobjeto);
	if(obj->tipo==ITEM && !obj->cerrada) {
		for(u1 k=0;k<obj->contenido.size;k++) {
			objvis(destino,arrget(obj->contenido,k));
		}
	}
}

Array psivis(u1 psi) {
	Array vis=arrnew();
	Objeto* opsi=objget(psi);
	Objeto* oloc=(opsi)?objget(opsi->contenedor):NULL;
	if(oloc) {
		for(u1 k=0;k<opsi->contenido.size;k++) {
			objvis(&vis,arrget(opsi->contenido,k));
		}
		for(u1 k=0;k<oloc->contenido.size;k++) {
			u1 noe=arrget(oloc->contenido,k);
			if(noe!=psi) {
				objvis(&vis,noe);
			}
		}
	}
	return vis;
}

#define isju(A) ((A)==idjugador)
#define juin(P,A) (idjugador!=0 && idjugador!=(P) && (objget(idjugador)->contenedor==(A)))


bool psimov(u1 psi,u1 adir) {
	char* salnom[]={"norte","oeste","abajo","arriba","este","sur"};
	Objeto* ojug=objget(idjugador);
	if(ojug) {
		Objeto* opsi=objget(psi);
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
						outnl(1);
					} else if(juin(psi,oloc)){
						out("%s va hacia %s",opsi->nombre,salnom[dir]);
						outnl(1);
					} else if(juin(psi,nloc)) {
						out("%s viene desde %s",opsi->nombre,salnom[SALIDAS-1-dir]);
						outnl(1);
					}
					return true;
				} else if(isju(psi)) {
					out("No hay salida visible en esa direccion...");
					outnl(1);
				}
			}
		}
	}
	return false;
}

static char* nomus;

static bool condnom(u1 nobj) {
	Objeto* obj=objget(nobj);
	if(obj && cadequ(nomus,obj->nombre,true)) return 1;
	return 0;
}

u1 objvisnom(u1 psi,char* nombre) {
	nomus=nombre;
	Array visto=psivis(psi);
	Array res=objcon(visto,condnom);
	if(res.size) return arrget(res,0);
	else return 0;
}

bool psicog(u1 psi,char* nombre_objeto) {
	char* men[]={	"Lo cojo",
					"No puedo coger nada mas...",
					"No veo eso...",
					"Eso no lo puedo coger...",
					"Ya lo tengo..."
	};
	u1 ret=0;
	Objeto* opsi=objget(psi);
	if(opsi) {
		if(opsi->contenido.size<opsi->capacidad) {
			Objeto* localidad=objget(opsi->contenedor);
			Objeto* objeto=objget(objvisnom(psi,nombre_objeto));
			if(objeto) {
				if(objeto->tipo==ITEM && objeto->cogible) {
					if(objeto->contenedor==psi) {
						ret=4;
					} else {
						objexp(objeto->id);
						objins(psi,objeto->id);
						if(juin(psi,localidad->id)) {
							out("%s coge %s...",opsi->nombre,nombre_objeto);
							outnl(1);
						}
					}
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
		Objeto* objeto=objget(objvisnom(psi,nombre_objeto));
		if(objeto && objeto->contenedor==psi) {
			objexp(objeto->id);
			objins(olocalidad->id,objeto->id);
			if(juin(psi,olocalidad->id)) {
				out("%s deja %s...",opsi->nombre,nombre_objeto);
				outnl(1);
			}
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
			haypsi=(oc->tipo==PSI) && (oc->amigo!=opsi->amigo) && (!oc->muerto);
		}
		if(!haypsi) {
			if(opsi->vida<MAXFEAT) opsi->vida+=APD;	
			else ret=2;
			if(juin(psi,oloc->id)) {
				out("%s descansa...",opsi->nombre);
				outnl(1);
			}
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
					"No puedo entrar ahi...",
	};
	u1 ret=0;
	Objeto* opsi=objget(psi);
	if(opsi) {
		Objeto* oloc=(opsi)?objget(opsi->contenedor):NULL;
		if(oloc) {
			Objeto* entrada=objget(objvisnom(psi,no));
			if(entrada) {
				if(entrada->tipo==LOCALIDAD && entrada->contenedor!=psi) {
					objexp(psi);
					objins(entrada->id,psi);
					if(juin(psi,oloc->id)) {
						out("%s entra en %s...",opsi->nombre,entrada->nombre);
						outnl(1);
					} else if(juin(psi,entrada->id)) {
						out("%s entra...",opsi->nombre);
						outnl(1);
					}
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
			if(juin(psi,oloc->id)) {
				out("%s sale...",opsi->nombre);
				outnl(1);
			}
			if(juin(psi,coloc->id)) {
				out("%s entra...",opsi->nombre);
				outnl(1);
			}
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
			u1 ob=arrget(opsi->contenido,k);
			objexp(ob);
			objins(nloc,ob);
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
		Objeto* riv=objget(objvisnom(psi,np));
		if(riv) {
			if(riv->tipo==PSI) {
				if(ojug==opsi) {
					out("Atacas a %s",riv->nombre);
					outnl(1);
				} else if(riv==ojug) {
					out("%s te ataca",opsi->nombre);
					outnl(1);
				} else if(jugloc==psiloc) {
					out("%s ataca a %s",opsi->nombre,riv->nombre);
					outnl(1);
				}
				if(riv->amigo) {
					if(ojug==opsi) {
						out("%s era amigo tuyo...",riv->nombre);
						outnl(1);
						riv->amigo=false;
					}
				}
				int difer=atatot(opsi)-atatot(riv);
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

static Array eneplc(Objeto* loc,bool amigo) {
	Array ene=arrnew();
	for(u1 k=0;k<loc->contenido.size;k++) {
		u1 no=arrget(loc->contenido,k);
		Objeto* ono=objget(no);
		if(ono->tipo==PSI && ono->amigo!=amigo) {
			arrpsh(&ene,no);
		}
	}
	return ene;
}

static bool enechk(Objeto* psi,Objeto* ene) {
	int hpsi=psi->destreza+rnd(1,6);
	int hene=ene->destreza+rnd(1,6);
	if(hpsi<=hene) {
		psiata(ene->id,psi->nombre);
		return false;
	}
	return true;
}

static bool eneschk(Objeto* psi,Array enemigos) {
	int escapa=true;
	if(enemigos.size>0) {
		for(u1 k=0;k<enemigos.size && escapa;k++) {
			u1 no=arrget(enemigos,k);
			Objeto* ono=objget(no);
			escapa=enechk(psi,ono);
		}
	}	
	return escapa;
}		

static bool salrnd(u1 psi,Objeto* loc) {
	u1 orden[256];
	char* complemento[256];
	u1 size=0;
	Objeto* cloc=objget(loc->contenedor);
	if(cloc && cloc->tipo==LOCALIDAD) {
		orden[size++]=ASALIR;
	}
	for(u1 k=0;k<loc->contenido.size;k++) {
		u1 ncon=arrget(loc->contenido,k);
		Objeto* ocon=objget(ncon);
		if(ocon->tipo==LOCALIDAD) {
			orden[size]=AENTRAR;
			complemento[size++]=ocon->nombre;
		}
	}
	for(u1 k=0;k<SALIDAS;k++) {
		if(loc->salida[k]!=0) {
			orden[size++]=k+1;
		}
	}
	if(size>0) {
		u1 sac=rnd(0,size-1);
		u1 ord=orden[sac];
		if(ord==AENTRAR) {
			return psient(psi,complemento[sac]);
		} else if(ord==ASALIR) {
			return psisal(psi);
		} else {
			return psimov(psi,ord);
		}
	} else {
		if(isju(psi)) {
			out("No hay salidas, no puedes escapar!!!");
			outnl(1);
		}
	}
	return true;
}

bool psihui(u1 psi) {
	Objeto* opsi=objget(psi);
	Objeto* oloc=(opsi)?objget(opsi->contenedor):NULL;
	if(oloc) {
		Array enemigos=eneplc(oloc,opsi->amigo);
		int escapa=eneschk(opsi,enemigos);
		if(escapa) {
			return salrnd(psi,oloc);
		} else {
			if(isju(psi)) {
				out("No has conseguido escapar...");
				outnl(1);
			} else if(juin(psi,oloc->id)) {
				out("%s ha intentado escapar pero no lo ha conseguido...",opsi->nombre);
				outnl(1);
			}
			return true;
		}
	}
	return false;
}

bool psinot(u1 psi) {
	Objeto* opsi=objget(psi);
	Objeto* oloc=(opsi)?objget(opsi->contenedor):NULL;
	if(oloc) {
		if(isju(psi)) {
			out("Te quedas petrificado...");
			outnl(1);
		} else if(juin(psi,oloc->id)) {
			out("%s se queda petrificado...",opsi->nombre);
			outnl(1);
		}
		return true;
	}
	return false;
}

bool psiexa(u1 psi,char* nombre) {
	if(isju(psi)) {
		Objeto* opsi=objget(psi);
		Objeto* oloc=(opsi)?objget(opsi->contenedor):NULL;
		if(oloc) {
			Objeto* oex=objget(objvisnom(psi,nombre));
			if(!oex) {
				out("No veo eso...");
			} else {
				objprt(oex->id);
			}
		}
	}
	return false;
}

bool psiinv(u1 psi) {
	if(isju(psi)) {
		Objeto* ojug=objget(psi);
		u1 size=ojug->contenido.size;
		if(!size) out("No llevas nada encima...");
		else {
			out("Tienes: ");
			outnl(1);
			for(u1 k=0;k<size;k++) {
				u1 nobj=ojug->contenido.data[k];
				Objeto* obj=objget(nobj);
				outtb(1);
				out("-%s",obj->nombre);
				outnl(1);
			}
		}
	}
	return false;
}

bool psiusa(u1 psi,char* oa,char* ob) {
	Objeto* opsi=objget(psi);
	if(opsi) {
		u1 noa,nob;
		noa=nob=0;
		for(u1 k=0;k<2;k++) {
			char* oe=(k==0)?oa:ob;
			if(*oe!='\0') {
				u1 noe=objvisnom(psi,oe);
				if(noe==0) {
					if(isju(psi)) {
						out("No puedo ver %s...",oe);
						outnl(1);
					}
					return false;
				} else {
					if(k==0) noa=noe;
					else nob=noe;
				}
			}
		}
		if(!usachk(noa,nob) && isju(psi)) {
			out("Nada sucede...");
			outnl(1);
		}
		return true;
	}
	return false;
}

bool psiact(u1 psi) {
	Objeto* opsi=objget(psi);
	if(opsi && opsi->tipo==PSI && !opsi->muerto && opsi->ia) {
		if(!opsi->ia(psi)) return psinot(psi);
	}
	return false;
}

bool conpsi(u1 psi) {
	Objeto* opsi=objget(psi);
	return (opsi && opsi->tipo==PSI && opsi->jugador==false && opsi->ia);
}

void pssact() {
	static Array psis;
	static bool defined=false;
	if(!defined) {
		psis=objsel(conpsi);
	}
	for(u1 k=0;k<psis.size;k++) {
		u1 psi=arrget(psis,k);
		psiact(psi);
	}
}
		


	




			
				









			








