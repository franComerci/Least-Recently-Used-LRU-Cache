#ifndef TDA_LISTA_DOBLE_H_INCLUDED
#define TDA_LISTA_DOBLE_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../comun.h"
#include "../TDA_ARBOL/TDA_ARBOL.h"
typedef struct sNodo2
{
    void *dato;
    unsigned tam;
    struct sNodo2 *sig;
    struct sNodo2 *ant;
}tNodo2;

typedef tNodo2 *tListaDoble;

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

void eliminarNodoDobleArbol(tListaDoble *pl, tNodo2 *nodo);

///FUNCIONES DE CACHE CON LISTA DOBLE
void crearLruCacheListaDobleyArbol(tCacheListaDoble *cache, size_t maxCap, size_t tamClave, size_t tamDato, Cmp cmp);
int agregarLruCacheListaDobleyArbol(tCacheListaDoble *cache, const void *clave, tElemCacheListaDoble *dato);
int obtenerLruCacheListaDobleyArbol(tCacheListaDoble *cache, const void *clave, void *dato);
int borrarLruCacheListaDobleyArbol(tCacheListaDoble *cache, const void *clave);
int vaciarLruCacheListaDobleyArbol(tCacheListaDoble *cache);
int destruirLruCacheListaDobleyArbol(tCacheListaDoble *cache);
int cacheListaDobleLlenayArbol(const tCacheListaDoble *cache);
int cacheListaDobleVaciayArbol(const tCacheListaDoble *cache);

#endif // TDA_LISTA_DOBLE_H_INCLUDED
