#ifndef TDA_LISTA_H_INCLUDED
#define TDA_LISTA_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MIN( X , Y ) ( ( X ) <= ( Y ) ? ( X ) : ( Y ) )


typedef struct sNodo
{
    void *info;
    size_t tamInfo;
    struct sNodo *sig;
}tNodo;

typedef tNodo *tLista;

void crearLista(tLista *pl);
int listaVacia(const tLista *pl);
int listaLlena(const tLista *pl, size_t cantBytes);
void vaciarLista(tLista *pl);
int ponerPrimeroLista(tLista *pl, const void *pd, size_t cantBytes);
int sacarPrimeroLista(tLista *pl, void *pd, size_t cantBytes);
int verPrimeroLista(tLista *pl, void *pd, size_t cantBytes);
int ponerUltimoLista(tLista *pl, const void *pd, size_t cantBytes);
int sacarUltimoLista(tLista *pl, void *pd, size_t cantBytes);
int verUltimoLista(tLista *pl, void *pd, size_t cantBytes);
/*
por si queremos cambiar extraerDeCache
int sacarDeLista(tLista *pl, void *pd, size_t cantBytes, const void *busq, Cmp cmp, void *params);
*/

#endif // TDA_LISTA_H_INCLUDED
