/*
============================================================
  Fichero: aventura.h
  Creado: 07-04-2025
  Ultima Modificacion: dimarts, 8 d’abril de 2025, 12:32:54
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

//CONSTANTES

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
#define AFINALIZAR 18

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
#define Ison flion
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




