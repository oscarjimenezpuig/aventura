/*
============================================================
  Fichero: psi.h
  Creado: 18-03-2025
  Ultima Modificacion: diumenge, 30 de març de 2025, 09:20:26
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "item.h"


bool psinew(u1 id,char* nombre,char* descripcion,bool jugador,bool amigo,u1 ataque,u1 destreza,u1 capacidad,IA ia);
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

bool psinot(u1 psi);
//el psi no hace nada (no hace falta introducirla en ia)

bool psiexa(u1 psi,char* nombre);
//examinar (accion que solo hace el jugador, retorno false para poder hacer otra accion)

bool psiinv(u1 psi);
//inventario (accion solo para el jugador, retorno false para poder hacer otra accion);

bool psiact(u1 psi);
// accion de psi que posee ia

void pssact();
// accion de todos los psis del juego que tienen ia


