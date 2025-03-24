/*
============================================================
  Fichero: psi.h
  Creado: 18-03-2025
  Ultima Modificacion: dilluns, 24 de març de 2025, 10:58:52
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "item.h"

bool psinew(u1 id,char* nombre,char* descripcion,bool jugador,bool amigo,u1 ataque,u1 destreza,u1 capacidad);
// define un nuevo psi

u1 psijug(); 
// da el identificador del jugador

bool psimov(u1 psi,u1 accion_direccion);
// mover el psi que esta en una localidad

bool psicog(u1 psi,char* nombre_objeto);
// coger un objeto que debe estar en la localizacion

bool psidej(u1 psi,char* nombre_objeto);
// dejar un objeto que debe estar en el psi
