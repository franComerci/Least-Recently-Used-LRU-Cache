#ifndef TDA_ARBOL_H_INCLUDED
#define TDA_ARBOL_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../comun.h"

#define reservarMemoriaNodo(X , Y , Z , W) (            \
                ( ( X ) = ( tNodo* )malloc( Y ) ) == NULL || \
                ( ( Z ) = malloc( W ) ) == NULL ? \
                    free(X), 0 : 1 )

#define MAXIMO(a, b) ((a) > (b) ? (a) : (b))

typedef struct sNodo
{
    void *dato;
    unsigned tam;
    struct sNodo *izq;
    struct sNodo *der;
} tNodo;

typedef tNodo *tArbol;

void    crearArbol(tArbol *pa);
int     insertarOrdenado(tArbol *pa, void *dato, unsigned tam, Cmp comparar);
int     eliminarNodo(tArbol *pa, void *dato, unsigned tam, Cmp comparar);
int     arbolVacio(const tArbol *pa);
int     arbolLleno(const tArbol *pa, unsigned tam);
void    vaciarArbol(tArbol *pa);
int     contarNodos(const tArbol *pa);
int     alturaArbol(const tArbol *pa);
void    recorrerPreorden(const tArbol *pa, tAccion accion);
int     compararCaracteres(const void *a, const void *b);
void    mostrarLista(void *a, void *b);
tArbol *buscarClave(tArbol *pa, Cmp comparar, void *clave);
void    intercambio(tArbol *elim, tArbol *paR);

#endif // TDA_ARBOL_H_INCLUDED
