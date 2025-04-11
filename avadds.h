/*
============================================================
  Fichero: avadds.h
  Creado: 10-04-2025
  Ultima Modificacion: divendres, 11 d’abril de 2025, 11:12:33
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

#define Laberinth labset
