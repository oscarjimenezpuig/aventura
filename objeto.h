/*
============================================================
  Fichero: objeto.h
  Creado: 16-03-2025
  Ultima Modificacion: divendres, 21 de març de 2025, 12:11:02
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "array.h"

#define NIL 0
#define LOCALIDAD 1
#define PSI 2

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
	Cadena nombre;
	Cadena descripcion;
	u1 contenedor;
	Array contenido;
	union {
		struct { //localidad
			u1 salida[SALIDAS];
		};
		struct { //psi
			u1 ataque,destreza,capacidad;
			bool jugador,muerto,amigo;
		};
	};
} Objeto;

typedef bool (*Condicion)(Objeto* objeto);

Objeto* objnew(u1 id,u1 tipo,char* nombre);
//define un nuevo objeto (el identificador 0 reservado para la localidad universo)2

Objeto* objget(u1 id);
//consigue el obejeto con identificador dado

bool objins(u1 receptor,u1 objeto);
//inserta un objeto en otro (siempre y cuando el objeto no este contenido en ningun sitio)

bool objexp(u1 objeto);
//quita el objeto de un contenedor

Array objsel(Condicion condicion);
//un array de idientificadores de los objetos que cumplen una determinada condicion

void objprt(u1 id);
//imprime un objeto segun el tipo que sea




