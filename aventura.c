/*
============================================================
  Fichero: aventura.c
  Creado: 07-04-2025
  Ultima Modificacion: dijous, 10 d’abril de 2025, 11:03:18
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

// INCLUDES

#include "aventura.h"

// CONSTANTES

#define TABSIZ 4 //espacios del tab
#define MAXFEAT 12 //maximo de cualquier caracteristica de pis
#define SINS 10 //numero de sinonimos maximo de cada accion
#define ACCIONES AFINALIZAR+1 //numero de acciones maximo
#define TPI MAXFEAT //numero total de puntos a repartir al inicio
#define BANDERAS 256 //numero de banderas
#define OBJETOS 256 //numero de objetos totales
#define EVENTOS 256 //numero de eventos totales
#define EUS 0 //eventos usar (array)
#define EAN 1 //eventos antes (array)
#define EDS 2 //eventos despues (array)
#define TERMW 80 //numero de columnas de la terminal

// TIPOS

typedef struct {
	u1 id;
	u1 sinonimos;
	Cadena verbo[SINS];
} Accion;

typedef struct {
	u1 orden;
	Cadena complemento[2];
} Token;

// VARIABLES
static bool bandera[BANDERAS];
static bool banderadef=false;
static Objeto objetos[OBJETOS];
static Evento eventos[EVENTOS];
static Array aev[3];
static Accion acciones[ACCIONES];
static u1 idjugador=0;
static char* nomus;
static Token lsttok={0};
static Objeto* ojugador=NULL;
static int turno=1;
static u1 finval=0;
static bool finis=false;
static bool aquienemigo=false;

// PROTOTIPOS FUNCION

void cadcpy(Cadena d,char* o);
// copia un string en una cadena */

bool cadequ(char* a,char* b,bool insensitive);
// mira si dos cadenas son iguales insensitive=true: case insensitive*/

bool cadsep(Cadena d,char** f,char separador);
// separa f y obtiene la primera palabra diferente al vacio, responde true si encuentra palabra*/

u1 cadlen(char* c);
// da la longitud de la cadena */

Array arrnew();
//nuevo array de enteros

u1 arrsiz(Array array);
//dar el tamaño del array

u1 arrget(Array array,u1 pos);
//conseguir el valor de una posicion

void arrclr(Array* array);
//limpiar el array a cero

bool arrpsh(Array* array,u1 value);
//introducir en la cola un valor

bool arrins(Array* array,u1 size,u1* values);
//instertar un array de valores

bool arrfnd(Array array,u1 value,u1* pos);
//buscar un valor

void arrera(Array* array,u1 pos);
//borrar una posicion

void arrprt(Array array);
//imprimir todos los valores del array

Objeto* objnew(u1 id,u1 tipo,char* nombre);
//define un nuevo objeto (el identificador 0 reservado para la localidad universo)2

Array objsel(Condicion condicion);
//un array de idientificadores de los objetos que cumplen una determinada condicion

Array objcon(Array array, Condicion condicion);
//un array de identificadores de los objetos de un array que cumplen la condicion

void objprt(u1 id);
//imprime un objeto segun el tipo que sea

bool usachk(u1 objeto_1,u1 objeto_2);
//chequea todos los eventos usar donde esten implicados objeto_1 y objeto_2

bool evachk();
//chequea todos los eventos antes, si devuelve true, entonces continua con la accion del jugador

void evdchk();
//chequea todos los eventos despues

u1 accfnd(char* cadena);
//busca una accion del deposito de acciones

Array psivis(u1 psi);
// se da todo lo que es visible para un psi

bool psimov(u1 psi,u1 accion_direccion);
// mover el psi que esta en una localidad

bool psicog(u1 psi,char* nombre_objeto);
// coger un objeto que debe estar en la localizacion

bool psidej(u1 psi,char* nombre_objeto);
// dejar un objeto que debe estar en el psi

bool psidsc(u1 psi);
// descansar (siempre y cuando no haya un psi enemigo en la misma localidad, recuperar punto de vida 

bool psient(u1 psi,char* nombre_objeto);
// entrada a una localidad contenida en localidad actual

bool psisal(u1 psi);
// salida a un contenedor

bool psimat(u1 psi);
// matar a un psi y vaciar el contenido del cadaver en la localidad

bool psiata(u1 psi,char* nombre_pis);
// se lanza un ataque a un psi

bool psihui(u1 psi);
// se huye en una direccion aleatoria si se puede

bool psinot(u1 psi);
//el psi no hace nada (no hace falta introducirla en ia)

bool psiexa(u1 psi,char* nombre);
//examinar (accion que solo hace el jugador, retorno false para poder hacer otra accion)

bool psiinv(u1 psi);
//inventario (accion solo para el jugador, retorno false para poder hacer otra accion);

bool psiusa(u1 psi,char* objeto_1,char* objeto_2);
//usar, solo para el personaje jugador, es necesario para usar, que los dos objetos esten visibles
//o sean poseidos

bool psistt(u1 psi);
//da el status, solo para el personaje jugador

bool psiact(u1 psi);
// accion de psi que posee ia

bool iamov(u1 psi);
//ia para permitir el movimiento de un psi

bool iacog(u1 psi);
//ia para coger cualquier objeto

bool iadej(u1 psi);
//ia para dejar cualquier objeto

bool iadsc(u1 psi);
//ia para descansar

bool iaatd(u1 psi);
//io para atacar o huir

bool juglst(u1* orden,char* complemento_1,char* complemento_2);
//obtiene el ultimo token valido introducido por el jugador

//DEFINICION DE FUNCIONES

int rnd(int a,int b) {
	static bool defined=false;
	if(!defined) {
		srand(time(NULL));
		defined=true;
	}
	int min=(a>b)?b:a;
	int max=(a>b)?a:b;
	int dif=max-min+1;
	return min+(rand()%dif);
}

char* in(Cadena cad) {
	char* ptr=cad;
	char c=0;
	while((c=getchar())!='\n' && ptr-cad<CADLEN) {
		*ptr++=c;
	}
	*ptr='\0';
	return cad;
}

void out(const char* cad,...) {
	va_list list;
	va_start(list,cad);
	vprintf(cad,list);
	va_end(list);
}

void outnl(u1 ls) {
	for(u1 k=0;k<ls;k++) printf("\n");
}

void outtb(u1 ts) {
	for(u1 k=0;k<ts;k++) for(u1 t=0;t<TABSIZ;t++) printf(" ");
}

void outat(Atributo a) {
	Cadena c;
	sprintf(c,"\033[%im",a);
	printf(c);
}

void cadcpy(Cadena d,char* o) {
	char* ptro=o;
	char* ptrd=d;
	while(ptrd-d<CADLEN && *ptro!='\0') {
		*ptrd++=*ptro++;
	}
	*ptrd='\0';
}

static void cptolw(char* a) {
	if(*a>='A' && *a<='Z') {
		*a=*a-'A'+'a';
	}
}

static bool chrequ(char a,char b,bool ins) {
	if(a==b) return true;
	else if(ins) {
		cptolw(&a);
		cptolw(&b);
		return chrequ(a,b,false);
	}
	return false;
}

bool cadequ(char* a,char* b,bool ins) {
	char* ptra=a;
	char* ptrb=b;
	while(*ptra!='\0') {
		if(!chrequ(*ptra,*ptrb,ins)) return false;
		ptra++;
		ptrb++;
	}
	return (*ptrb=='\0')?true:false;
}

bool cadsep(Cadena d,char** f,char s) {
	char* ptrd=d;
	char* ptrf=*f;
	while(*ptrf!=s && *ptrf!='\0') {
		*ptrd++=*ptrf++;
	}
	*ptrd='\0';
	if(*ptrf==s) ptrf++;
	*f=ptrf;
	if(*d=='\0') {
		if(*ptrf=='\0') return false;
		else return cadsep(d,f,s);
	} else return true;
}

u1 cadlen(char* c) {
	char* ptrc=c;
	while(*ptrc!='\0') ptrc++;
	return (u1)(ptrc-c);
}

static void flginit() {
	bool* ptr=bandera;
	while(ptr!=bandera+BANDERAS) {
		*ptr++=false;
	}
}

#define defflg if(!banderadef && (banderadef=true)) flginit()

static void flgset(u1 flag,bool value) {
	defflg;
	bandera[flag]=value;
}

void flgon(u1 f) {
	flgset(f,true);
}

void flgoff(u1 f) {
	flgset(f,false);
}

void flgnot(u1 f) {
	defflg;
	bool* pf=bandera+f;
	*pf=!(*pf);
}

bool flgion(u1 f) {
	defflg;
	return bandera[f];
}

#undef defflg

Array arrnew() {
	Array a;
	a.size=0;
	u1* ptra=a.data;
	while(ptra!=a.data+ARRSIZE) {
		*ptra++=0;
	}
	return a;
}

u1 arrsiz(Array a) {
	return a.size;
}

u1 arrget(Array a,u1 pos) {
	return a.data[pos];
}

void arrclr(Array* a) {
	a->size=0;
}

bool arrpsh(Array* a,u1 v) {
	if(a->size<ARRSIZE) {
		a->data[a->size++]=v;
		return true;
	}
	return false;
}

bool arrins(Array* a,u1 s,u1* v) {
	bool ret=true;
	for(u1 k=0;ret && k<s;k++) {
		ret=arrpsh(a,v[k]);
	}
	return ret;
}

bool arrfnd(Array a,u1 v,u1* p) {
	u1 pos=0;
	for(pos=0;pos<a.size;pos++) {
		if(a.data[pos]==v) {
			*p=pos;
			return true;
		}
	}
	return false;
}

void arrera(Array* a,u1 p) {
	for(u1 k=p;k<a->size-1;k++) {
		a->data[k]=a->data[k+1];
	}
	a->size--;
}

void arrprt(Array a) {
	printf("{ ");
	for(u1 k=0;k<a.size;k++) {
		printf("%i ",a.data[k]);
	}
	printf("} #%i\n",a.size);
}

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

Array objcon(Array a,Condicion c) {
	Array res=arrnew();
	for(u1 k=0;k<a.size;k++) {
		u1 o=arrget(a,k);
		if(c(o)) {
			arrpsh(&res,o);
		}
	}
	return res;
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
		aquienemigo=true;
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
	} else aquienemigo=false;
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
			outnl(1);
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
	if(o->cerrada) out("Esta cerrado.");
	else if(o->contenido.size) {
		out("En su interior puedes ver: ");
		for(u1 k=0;k<o->contenido.size;k++) {
			out("%s ",objget(arrget(o->contenido,k))->nombre);
		}
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

bool locnew(u1 i,char* n,char* d) {
	Objeto* o=objnew(i,LOCALIDAD,n);
	if(o) {
		cadcpy(o->descripcion,d);
		for(u1 k=0;k<SALIDAS;k++) o->salida[k]=0;
		o->visitado=false;
	}
	return (o==NULL)?false:true;
}

bool loccon(u1 o,u1 d,u1 s,bool b) {
	Objeto* oo=objget(o);
	Objeto* od=objget(d);
	if(oo && od && oo->tipo==LOCALIDAD && od->tipo==LOCALIDAD) {
		oo->salida[s]=d;
		if(b) od->salida[SALIDAS-s-1]=o;
		return true;
	}
	return false;
}
		
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
	accnew(14,"descansar");
	accnew(15,"inventario");
	accnew(16,"nada");
	accnew(17,"examinar");
	accins(17,"ex");
	accnew(18,"status");
	accins(19,"fin");
}

u1 accfnd(char* a) {
	static bool defined=false;
	if(!defined && (defined=true)) accdef();
	for(u1 id=1;id<ACCIONES;id++) {
		Accion* ac=acciones+id;
		for(u1 s=0;s<ac->sinonimos;s++) {
			if(cadequ(a,ac->verbo[s],true)) return id;
		}
	}
	return ANULL;
}

bool itmnew(u1 id,char* n,char* d,bool c,bool cr,u1 p) {
	Objeto* o;
	if((o=objnew(id,ITEM,n))) {
		if(d) cadcpy(o->descripcion,d);
		o->cogible=c;
		o->plus=p;
		o->cerrada=cr;
		return true;
	}
	return false;
}

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
			if(j && !ojugador) {
				idjugador=i;
				ojugador=objget(i);
			}
			idjugador=(j)?i:idjugador;
			opsi->muerto=false;
			opsi->ia=ia;
			return true;
		}
	}
	return false;
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
#define juin(P,A) (idjugador!=0 && idjugador!=(P) && (ojugador->contenedor==(A)))

bool psimov(u1 psi,u1 adir) {
	char* salnom[]={"norte","oeste","abajo","arriba","este","sur"};
	Objeto* ojug=ojugador;
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
			u1 locpsi=ojugador->contenedor;
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
	Objeto* ojug=ojugador;
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

bool psistt(u1 psi) {
	char* svida[]={	"muy malo",
					"malo",
					"regular",
					"bueno",
					"muy bueno"
	};
	Objeto* opsi=objget(psi);
	if(opsi && opsi->jugador) {
		out("%s, tus caracteristicas son AT:%i y HB:%i. Llevas %i objetos de %i posibles.",opsi->nombre,opsi->ataque,opsi->destreza,opsi->contenido.size,opsi->capacidad);
		outnl(1);
		u1 v=opsi->vida;
		u1 c=(v<2)?0:(v<4)?1:(v<6)?2:(v<8)?3:4;
		out("Tu estado de salud es %s.",svida[c]);
		outnl(1);
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

#undef isju
#undef juin

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

static u1 psiataval(Objeto* psi) {
	u1 maxval=0;
	for(u1 k=0;k<psi->contenido.size;k++) {
		u1 nobj=arrget(psi->contenido,k);
		Objeto* obj=objget(nobj);
		if(obj->tipo==ITEM && obj->plus>maxval) maxval=obj->plus;
	}
	return maxval+psi->ataque;
}

static bool psiatac(Objeto* psi) {
	Array vis=psivis(psi->id);
	int favor=psiataval(psi);
	int contra=0;
	for(u1 k=0;k<vis.size;k++) {
		Objeto* ovis=objget(arrget(vis,k));
		if(ovis->tipo==PSI) {
			u1 ataval=psiataval(ovis);
			if(ovis->amigo==psi->amigo) favor+=ataval;
			else contra+=ataval;
		}
	}
	return (favor>=contra);
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

static void reppoint(u1* p) {
	char* car[]={"ATAQUE","DESTREZA","CAPACIDAD"};
inicio:
	u1 pun=TPI;
	for(u1 k=0;k<3 && pun>0;k++) {
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
	out("En resumen: AT:%i HB:%i CP:%i",p[0],p[1],p[2]);
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
	out("Hola %s, vamos a definir tus caracteristicas...",s);
	outnl(1);
	u1 pun[]={0,0,0};
	reppoint(pun);
	psinew(id,s,"",true,true,pun[0],pun[1],pun[2],NULL);
	return (ojugador!=NULL);
}

static u1 separa(Cadena orden,Cadena* frase) {
	char* ptro=orden;
	u1 palabras=0;
	while(*ptro!='\0' && palabras<3) {
		if(cadsep(frase[palabras],&ptro,' ')) palabras++;
	}
	return palabras;
}

static bool atncene() {
	out("No puedes, aqui hay enemigos...");
	outnl(1);
	return false;
}

static bool actsep(Token tok) {
	if(aquienemigo) {
		switch(tok.orden) {
			case AATACAR:
				return psiata(idjugador,tok.complemento[0]);
			case AHUIR:
				return psihui(idjugador);
			case AEXAMINAR:
				return psiexa(idjugador,tok.complemento[0]);
			case AUSAR:
				return psiusa(idjugador,tok.complemento[0],tok.complemento[1]);
			case ASTATUS:
				return psistt(idjugador);
			case AFINALIZAR:
				finset(FINQUIT);
			default:
				return atncene();
		}
	}						
	switch(tok.orden) {
		case ANORTE:
		case ASUR:
		case AESTE:
		case AOESTE:
		case AARRIBA:
		case AABAJO:
			return psimov(idjugador,tok.orden);
		case ACOGER:
			return psicog(idjugador,tok.complemento[0]);
		case ADEJAR:
			return psidej(idjugador,tok.complemento[0]);
		case AINVENTARIO:
			return  psiinv(idjugador);
		case ADESCANSAR:
			return psidsc(idjugador);
		case AENTRAR:
			return psient(idjugador,tok.complemento[0]);
		case ASALIR:
			return psisal(idjugador);
		case AATACAR:
			return psiata(idjugador,tok.complemento[0]);
		case AHUIR:
			return psihui(idjugador);
		case AEXAMINAR:
			return psiexa(idjugador,tok.complemento[0]);
		case AUSAR:
			return psiusa(idjugador,tok.complemento[0],tok.complemento[1]);
		case ASTATUS:
			return psistt(idjugador);
		case AFINALIZAR:
			finset(FINQUIT);
			return true;
		default:
			return false;
	}
}

static void jugvis() {
	//establece la localidad actual como visitada
	Objeto* oloc=objget(ojugador->contenedor);
	if(oloc) oloc->visitado=true;
}

bool jugact() {
	bool ok=false;
	if(ojugador && !ojugador->muerto) {
		jugvis();
		while(!ok) {
			out("Que quieres hacer? ");
			Cadena orden;
			in(orden);
			Cadena token[3];
			u1 tokens=separa(orden,token);
			if(tokens) {
				Token tok=toknew(0,"","");
				if((tok.orden=accfnd(token[0]))) {
					for(u1 k=0;k<tokens-1;k++) {
						cadcpy(tok.complemento[k],token[k+1]);
					}
					lsttok=tok;
					if(evachk()) { //chequeo de eventos antes
						if((ok=actsep(tok))) {
							evdchk(); //chequeo de eventos despues
						}
					} else ok=true;
				} else {
					out("No entiendo...");
				}
			}
			outnl(1);
		}
	}
	return ok;	
}

bool juglst(u1* o,char* c1,char* c2) {
	if(lsttok.orden!=0) {
		*o=lsttok.orden;
		cadcpy(c1,lsttok.complemento[0]);
		cadcpy(c2,lsttok.complemento[1]);
		return true;
	}
	return false;
}

int trnget() {
	return turno;
}

void trnpss() {
	++turno;
}

void finset(u1 v) {
	finval=v;
	finis=true;
}

bool finget(u1* v) {
	if(finis) *v=finval;
	return finis;
}

static bool condloc(u1 idobj) {
	Objeto* o=objget(idobj);
	if(o && o->tipo==LOCALIDAD) return true;
	return false;
}

void finfinprt() {
	char* FIN="FIN";
	u1 visitado=0;
	u1 total=0;
	Array localidades=objsel(condloc);
	total=localidades.size;
	for(u1 k=0;k<localidades.size;k++) {
		Objeto* loc=objget(localidades.data[k]);
		if(loc->visitado) visitado++;
	}
	if(total) {
		u1 porcentaje=(visitado*100)/total;
		out("Has descubierto el %i%% del mundo.",porcentaje);
		outnl(1);
	}
	u1 inprt=(TERMW-cadlen(FIN))/2;
	for(u1 k=0;k<inprt;k++) out(" ");
	outat(BOLD);
	out("FIN");
	outnl(1);
	outat(NORMAL);
}	

void visscr() {
	Objeto* jug=ojugador;
	u1 njugloc=0;
	if(!jug->muerto && (njugloc=jug->contenedor)) {
		Objeto* jugloc=objget(njugloc);
		outat(INVERSE);
		out("%-70s TURNO %03i",jugloc->nombre,turno);
		outat(NORMAL);
		outnl(1);
		objprt(njugloc);
		outnl(1);
	}
}
