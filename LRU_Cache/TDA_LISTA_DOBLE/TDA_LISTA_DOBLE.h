#ifndef TDA_LISTA_DOBLE_H_INCLUDED
#define TDA_LISTA_DOBLE_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../comun.h"

typedef struct sNodo2
{
    void *dato;
    unsigned tam;
    struct sNodo2 *sig;
    struct sNodo2 *ant;
}tNodo2;

typedef tNodo2 *tListaDoble;


///PRIMITIVAS LISTA DOBLE

void crearListaDoble(tListaDoble *pl);
int  insertarOrdenadoDoble(tListaDoble *pl, const void *dato, unsigned tam, Cmp comparar);
int  eliminarNodoDoble(tListaDoble *pl, void *dato, unsigned tam, Cmp comparar);
int  listaLlenaDoble(const tListaDoble *pl, unsigned tam);
int  listaVaciaDoble(const tListaDoble *pl);
void vaciarListaDoble(tListaDoble *pl);

//void eliminarNodoDobleArbol(tListaDoble *pl, tNodo2 *nodo);

#endif // TDA_LISTA_DOBLE_H_INCLUDED
