/*
============================================================
  Fichero: inout.c
  Creado: 17-03-2025
  Ultima Modificacion: dilluns, 24 de març de 2025, 11:28:59
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

void outat(Atributo a) {
	Cadena c;
	sprintf(c,"\033[%im",a);
	printf(c);
}

void cadcpy(Cadena d,char* o) {
	char* ptro=o;
	char* ptrd=d;
	while(ptrd-d<CADLEN && *ptro!='\0') {
		*ptrd++=*ptro++;
	}
	*ptrd='\0';
}

static bool chrequ(char a,char b,bool ins) {
	if(a==b) return true;
	else if(ins) {
		if(a>='A' && a<='Z') a=a-'A'+'a';
		else if(b>='A' && b<='Z') b=b-'A'+'a';
		return chrequ(a,b,ins);
	}
	return false;
}

bool cadequ(char* a,char* b,bool ins) {
	char* ptra=a;
	char* ptrb=b;
	while(*ptra!='\0') {
		if(!chrequ(*ptra,*ptrb,ins)) return false;
		ptra++;
		ptrb++;
	}
	return (*ptrb=='\0')?true:false;
}

bool cadsep(Cadena d,char** f,char s) {
	char* ptrd=d;
	char* ptrf=*f;
	while(*ptrf!=s && *ptrf!='\0') {
		*ptrd++=*ptrf++;
	}
	*ptrd='\0';
	if(*ptrf==s) ptrf++;
	*f=ptrf;
	if(*d=='\0') {
		if(*ptrf=='\0') return false;
		else return cadsep(d,f,s);
	} else return true;
}
