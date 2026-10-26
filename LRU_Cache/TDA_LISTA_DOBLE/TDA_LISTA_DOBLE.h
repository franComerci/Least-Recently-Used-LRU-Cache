#ifndef TDA_LISTA_DOBLE_H_INCLUDED
#define TDA_LISTA_DOBLE_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../comun.h"
#include "../TDA_ARBOL/TDA_ARBOL.h"
typedef struct sNodo
{
    void *dato;
    unsigned tam;
    struct sNodo *sig;
    struct sNodo *ant;
}tNodo;

typedef tNodo *tListaDoble;

typedef struct
{
    tListaDoble listaDoble;
    tArbol arbol;
    int cap;
    int ce;
    size_t tamClave;
    size_t tamDato;
    Cmp cmp;
}tCacheListaDoble;

typedef struct
{
    void *dato;//contenido
    void *clave;//para buscar
}tElemCacheListaDoble;//para guardar en la lista

///PRIMITIVAS LISTA DOBLE

void crearListaDoble(tListaDoble *pl);
int  insertarOrdenadoDoble(tListaDoble *pl, const void *dato, unsigned tam, Cmp comparar);
int  eliminarNodoDoble(tListaDoble *pl, void *dato, unsigned tam, Cmp comparar);
int  listaLlenaDoble(const tListaDoble *pl, unsigned tam);
int  listaVaciaDoble(const tListaDoble *pl);
void vaciarListaDoble(tListaDoble *pl);

///FUNCIONES DE CACHE CON LISTA DOBLE
void crearLruCacheListaDoble(tCacheListaDoble *cache, size_t maxCap, size_t tamClave, size_t tamDato, Cmp cmp);
int agregarLruCacheListaDoble(tCacheListaDoble *cache, const void *clave, tElemCacheListaDoble *dato);
int obtenerLruCacheListaDoble(tCacheListaDoble *cache, const void *clave, void *dato);
int borrarLruCacheListaDoble(tCacheListaDoble *cache, const void *clave);
int vaciarLruCacheListaDoble(tCacheListaDoble *cache);
int destruirLruCacheListaDoble(tCacheListaDoble *cache);
int cacheListaDobleLlena(const tCacheListaDoble *cache);
int cacheListaDobleVacia(const tCacheListaDoble *cache);

#endif // TDA_LISTA_DOBLE_H_INCLUDED
