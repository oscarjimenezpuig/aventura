/*
============================================================
  Fichero: objeto.h
  Creado: 16-03-2025
  Ultima Modificacion: diumenge, 16 de març de 2025, 13:24:34
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "array.h"

#define NIL 0
#define LOCALIDAD 1

#define NORTE 0
#define OESTE 1
#define ABAJO 2
#define ARRIBA 3
#define ESTE 4
#define SUR 5

#define SALIDAS 6

typedef struct {
	u1 id;
	u1 tipo;
	char* nombre;
	char* descripcion;
	u1 contenedor;
	Array contenido;
	union {
		struct { //localidad
			u1 salida[SALIDAS];
		};
	};
} Objeto;

typedef bool (*Condicion)(Objeto* objeto);

Objeto* objnew(u1 id,u1 tipo,char* nombre);
//define un nuevo objeto (el identificador 0 reservado para la localidad universo)2

Objeto* objget(u1 id);
//consigue el obejeto con identificador dado

Array objsel(Condicion condicion);
//un array de idientificadores de los objetos que cumplen una determinada condicion




