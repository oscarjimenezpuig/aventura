/*
============================================================
  Fichero: aventura.h
  Creado: 07-04-2025
  Ultima Modificacion: dijous, 10 d’abril de 2025, 11:01:56
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

//includes
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <stdarg.h>

//tipos de objeto
#define NIL 0
#define LOCALIDAD 1
#define PSI 2
#define ITEM 3

//direcciones en un objeto localidad
#define NORTE 0
#define OESTE 1
#define ABAJO 2
#define ARRIBA 3
#define ESTE 4
#define SUR 5

//tipos de eventos
#define USAR 1 //evento que se produce cuando se usan dos objetos
#define ANTES 2 //evento que se produce antes de la pregunta al jugador
#define DESPUES 4 //evento que se produce despues de la pregunta del jugador

//acciones 
#define ANULL 0
#define ANORTE 1
#define AOESTE 2
#define AABAJO 3
#define AARRIBA 4
#define AESTE 5
#define ASUR 6
#define ACOGER 7
#define ADEJAR 8
#define AENTRAR 9
#define ASALIR 10
#define AUSAR 11
#define AATACAR 12
#define AHUIR 13
#define ADESCANSAR 14
#define AINVENTARIO 15
#define ANADA 16
#define AEXAMINAR 17
#define ASTATUS 18
#define AFINALIZAR 19

//finales
#define FINQUIT 0 //final cuando se conecta quit
#define FINLOST 1 //final si jugador muere


//dimensiones
#define CADLEN 255 //longitud de cadena
#define ARRSIZE 256 //dimension maxima del array
#define SALIDAS 6 //numero de salidas de cada objeto

//TIPOS

typedef unsigned char u1;

typedef enum {false,true} bool;

typedef char Cadena[CADLEN+1];

typedef enum {NORMAL=0,BOLD=1,INVERSE=7} Atributo;

typedef struct {
	u1 size;
	u1 data[ARRSIZE];
} Array;

typedef bool (*IA)(u1);

typedef struct {
	u1 id;
	u1 tipo;
	Cadena nombre;
	Cadena descripcion;
	u1 contenedor;
	Array contenido;
	union {
		struct { //localidad
			u1 salida[SALIDAS];
			bool visitado;
		};
		struct { //psi
			u1 ataque,destreza,capacidad,vida;
			bool jugador,muerto,amigo;
			IA ia;
		};
		struct { //item
			bool cogible;
			bool cerrada; //si es cerrada, actua como caja
			u1 plus;
		};
		
	};
} Objeto;

typedef bool (*Condicion)(u1 idobj);

typedef bool (*Actuacion)(u1); //funcion que realiza el evento si esta activo (se pasa el identificador del evento)

typedef struct {
	u1 id;
	u1 tipo;
	u1 objeto[2];
	bool activo;
	Actuacion actuacion;
} Evento;

//PROTOTIPOS DE FUNCIONES

int rnd(int a,int b);
//aleatorio entre a y b incluidos

char* in(Cadena cadena);
//entrada teclado

void out(const char* cadena,...);
//impresion formateada

void outnl(u1 lines);
// nueva linea */

void outtb(u1 tab);
// tabulacion, a base de espacios */

void outat(Atributo a);
// attributos de escritura */

void flgon(u1 flag);
//cpmecta bandera

void flgoff(u1 flag);
//desconecta bandera

void flgnot(u1 flag);
//hace la inversa del valor de la bandera

bool flgion(u1 flag);
//mira si la bandera esta conectada

Objeto* objget(u1 id);
//consigue el obejeto con identificador dado

bool objins(u1 receptor,u1 objeto);
//inserta un objeto en otro (siempre y cuando el objeto no este contenido en ningun sitio)

bool objexp(u1 objeto);
//quita el objeto de un contenedor

Evento* evunew(u1 id,u1 objeto_1,u1 objeto_2,Actuacion actuacion);
//definicion de una accion usar (solo si id esta vacio)

Evento* evanew(u1 id,Actuacion actuacion);
//definicion de un evento antes (solo si id esta vacio)

Evento* evdnew(u1 id,Actuacion actuacion);
//definicion de un evento despues (solo si id esta vacio)

void evndel(u1 id);
//borrado de un evento

Evento* eveget(u1 id);
//apuntador al evento

bool locnew(u1 id,char* nombre,char* descripcion);
//nueva localidad

bool loccon(u1 origen,u1 destino,u1 salida,bool bidireccional);
//conecta dos localidades por una salida (bidireccional si hay que hacerlo en las dos direcciones)

bool itmnew(u1 id,char* nombre,char* descripcion,bool cogible,bool cerrada,u1 plus);
//definimos un item nuevo

bool psinew(u1 id,char* nombre,char* descripcion,bool jugador,bool amigo,u1 ataque,u1 destreza,u1 capacidad,IA ia);
// define un nuevo psi

void pssact();
// accion de todos los psis del juego que tienen ia

bool IAhum(u1 psi);
//ia completa de un psi humanoide

bool IAani(u1 psi);
//ia de un animal (no coge ni deja objetos)

bool IApln(u1 psi);
//ia de una planta (no se mueve, no coge ni deja objetos);

bool jugnew(u1 id);
//se define un nuevo jugador

bool jugact();
//se realizan y analizan acciones

int trnget();
// da el valor del turno

void trnpss();
//incrementa el turno en 1

int trnget();
//dar el turno actual

void finset(u1 value);
//pone la señal de final con un valor

bool finget(u1* value);
//da true si hay final

void finfinprt();
//da el final total con el tanto por ciento de mapa visitado y la palabra fin

void visscr();
// visibiliza la pantalla

//PROTOTIPOS FUNCIONES PRINCIPALES

#define Rnd rnd
#define In in
#define Out out
#define Newline outnl(1)
#define Tab outtb(1)
#define Attr outat
#define On flgon
#define Off floff
#define Not flgnot
#define Ison flgion
#define Get objget
#define Insert objins
#define Expulse objexp
#define Use evunew
#define Before evanew
#define After evdnew
#define Event eveget
#define Delete evndel
#define Room locnew
#define Conect loccon
#define Item itmnew
#define Npi psinew
#define Npiact pssact()
#define IAhuman
#define IAanimal
#define IAPlant
#define Player jugnew
#define Playeract jugact()
#define Torninc trnpss()
#define Torn trnget()
#define EndSet finset
#define Isend finget
#define Visible visscr()
#define Endout finfinprt()




