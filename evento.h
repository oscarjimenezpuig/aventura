/*
============================================================
  Fichero: evento.h
  Creado: 31-03-2025
  Ultima Modificacion: dimarts, 1 d’abril de 2025, 11:50:07
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "objeto.h"

#define USAR 1 //evento que se produce cuando se usan dos objetos
#define ANTES 2 //evento que se produce antes de la pregunta al jugador
#define DESPUES 4 //evento que se produce despues de la pregunta del jugador

typedef bool (*Actuacion)(u1); //funcion que realiza el evento si esta activo (se pasa el identificador del evento)

typedef struct {
	u1 id;
	u1 tipo;
	u1 objeto[2];
	bool activo;
	Actuacion actuacion;
} Evento;

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

bool usachk(u1 objeto_1,u1 objeto_2);
//chequea todos los eventos usar donde esten implicados objeto_1 y objeto_2

bool evachk();
//chequea todos los eventos antes, si devuelve true, entonces continua con la accion del jugador

void evdchk();
//chequea todos los eventos despues


	
