/*
============================================================
  Fichero: psi.h
  Creado: 18-03-2025
  Ultima Modificacion: dimarts, 18 de març de 2025, 12:27:39
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "acciones.h"

bool psinew(u1 id,char* nombre,char* descripcion,bool jugador,u1 ataque,u1 destreza,u1 capacidad);
// define un nuevo psi

u1 psijug(); 
// da el identificador del jugador

bool psipos(u1 psi,u1 loc);
//situar un objeto en una posicion (solo si no esta posicionado)

bool psimov(u1 psi,u1 dir);
// mover el psi que esta en una localidad
