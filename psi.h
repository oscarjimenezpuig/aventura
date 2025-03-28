/*
============================================================
  Fichero: psi.h
  Creado: 18-03-2025
  Ultima Modificacion: dijous, 27 de març de 2025, 14:36:38
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "item.h"

#define MAXVIDA 10 // maximo de vida de un psi

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

bool psidsc(u1 psi);
// descansar (siempre y cuando no haya un psi enemigo en la misma localidad, recuperar punto de vida 

bool psient(u1 psi,char* nombre_objeto);
// entrada a una localidad contenida en localidad actual

bool psisal(u1 psi);
// salida a un contenedor

bool psimat(u1 psi);
// matar a un psi y vaciar el contenido del cadaver en la localidad

bool psiata(u1 psi,char* nombre_pis);
// se lanza un ataque a un psi

bool psihui(u1 psi);
// se huye en una direccion aleatoria si se puede



