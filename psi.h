/*
============================================================
  Fichero: psi.h
  Creado: 18-03-2025
  Ultima Modificacion: dissabte, 22 de març de 2025, 20:59:31
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "acciones.h"

bool psinew(u1 id,char* nombre,char* descripcion,bool jugador,bool amigo,u1 ataque,u1 destreza,u1 capacidad);
// define un nuevo psi

u1 psijug(); 
// da el identificador del jugador

bool psimov(u1 psi,u1 accion_direccion);
// mover el psi que esta en una localidad
