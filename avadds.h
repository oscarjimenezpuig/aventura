/*
============================================================
  Fichero: avadds.h
  Creado: 10-04-2025
  Ultima Modificacion: dijous, 10 d’abril de 2025, 10:59:40
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
	u1 idlst;
	u1 w,h;
	u1 altura;
	u1 escaleras;
} Laberinto;

// nombre: nombre de la habitacion
// descripcion: descripcion de las habitacion
// idini: primer identificador de las habitaciones (a partir de ahí se construyen todas)
// idlst: ultimo identificador de las habitaciones (dado por el programa)
// w,h: dimension de cada planta
// altura: altura del piso
// escaleras: escaleras de subida por piso
// semilla: semilla que genera el laberinto

bool labset(Laberinto* laberinto);
//construye el laberinto
