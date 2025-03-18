/*
============================================================
  Fichero: inout.c
  Creado: 17-03-2025
  Ultima Modificacion: dilluns, 17 de març de 2025, 11:17:23
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "inout.h"

char* in(Cadena cad) {
	char* ptr=cad;
	char c=0;
	while((c=getchar())!='\n' && ptr-cad<CADLEN) {
		*ptr++=c;
	}
	*ptr='\0';
	return cad;
}

void out(const char* cad,...) {
	va_list list;
	va_start(list,cad);
	vprintf(cad,list);
	va_end(list);
}

void outnl(u1 ls) {
	for(u1 k=0;k<ls;k++) printf("\n");
}

void outtb(u1 ts) {
	for(u1 k=0;k<ts;k++) for(u1 t=0;t<TABSIZ;t++) printf(" ");
}

void cadcpy(Cadena d,char* o) {
	char* ptro=o;
	char* ptrd=d;
	while(ptrd-d<CADLEN && *ptro!='\0') {
		*ptrd++=*ptro++;
	}
	*ptrd='\0';
}

bool cadequ(char* a,char* b) {
	char* ptra=a;
	char* ptrb=b;
	while(*ptra!='\0') {
		if(*ptra!=*ptrb) return false;
		ptra++;
		ptrb++;
	}
	return (*ptrb=='\0')?true:false;
}


