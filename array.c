/*
============================================================
  Fichero: array.c
  Creado: 16-03-2025
  Ultima Modificacion: dimarts, 18 de març de 2025, 14:33:38
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "array.h"

Array arrnew() {
	return (Array){0};
}

void arrclr(Array* a) {
	a->size=0;
}

bool arrpsh(Array* a,u1 v) {
	if(a->size<ARRSIZE) {
		a->data[a->size++]=v;
		return true;
	}
	return false;
}

bool arrins(Array* a,u1 s,u1* v) {
	bool ret=true;
	for(u1 k=0;ret && k<s;k++) {
		ret=arrpsh(a,v[k]);
	}
	return ret;
}

bool arrfnd(Array a,u1 v,u1* p) {
	u1 pos=0;
	for(pos=0;pos<a.size;pos++) {
		if(a.data[pos]==v) {
			*p=pos;
			return true;
		}
	}
	return false;
}

void arrera(Array* a,u1 p) {
	for(u1 k=p;k<a->size-1;k++) {
		a->data[k]=a->data[k+1];
	}
	a->size--;
}

void arrprt(Array a) {
	printf("{ ");
	for(u1 k=0;k<a.size;k++) {
		printf("%i ",a.data[k]);
	}
	printf("} #%i\n",a.size);
}


