/*
============================================================
  Fichero: avadds.h
  Creado: 10-04-2025
  Ultima Modificacion: dilluns, 14 d’abril de 2025, 05:57:51
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

//Extension de aventura que crea laberintos tridimensionales con habitaciones no diferenciadas

#include "aventura.h"

#define CADENAS 256

typedef struct {
	char* nombre;
	char* descripcion;
	u1 idini;
	u1 w,h;
	u1 altura;
	u1 escaleras;
	bool numnom;
	bool levdes;
} Laberinto;

// nombre: nombre de la habitacion
// descripcion: descripcion de las habitacion
// idini: primer identificador de las habitaciones (a partir de ahí se construyen todas)
// w,h: dimension de cada planta
// altura: altura del piso
// escaleras: escaleras de subida por piso
// numnom: añade un numero al nombre
// levdes: añade el piso en la descripcion

void labset(Laberinto laberinto);
//construye el laberinto

typedef struct {
	u1 min,max;
} Intervalo;

typedef struct {
	char* nombre;
	Intervalo letras;
} Nombre;
//Si nombre es nulo, se crea un nombre entre el minimo y maximo de letras

typedef struct {
	bool todo;
	Array localidades;
} Zona;
//si todo es falso, se leen las localidades donde se puede poner todos los elementos

typedef struct {
	u1 idini;
	Nombre nombre;
	char* descripcion;
	bool amigo;
	Intervalo ataque,destreza,capacidad;
	IA ia;
	Zona zona;
} Raza;

// idini: primer id que utilizaran 
// nombre: da un nombre y genera un aleatorio
// descripcion: descripcion
// ataque, destreza y vida: intervalo de las caracteristicas
// ia: ia que tiene

u1 raznew(Raza raza,u1 members);

#define Laberinth labset
#define Race raznew
