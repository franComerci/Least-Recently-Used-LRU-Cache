#ifndef TDA_CACHE_LISTA_H_INCLUDED
#define TDA_CACHE_LISTA_H_INCLUDED

#include "../TDA_LISTA/TDA_Lista.h"

typedef struct
{
    tLista lista;
    int cap;
    int ce;
    size_t tamClave;
    size_t tamDato;
}tCache;

typedef struct
{
    void *dato;
    void *clave;
}tElemCache;//usamos esta struct para guardar en la lista

typedef int (*Cmp)(const void *a, const void *b);


void crearLruCache(tCache *cache, size_t maxCap, size_t tamClave, Cmp cmp);
int agregarLruCache(tCache *cache, const void *clave, const void *dato, Cmp cmp);
int obtenerLruCache(tCache *cache, const void *clave, void *dato, Cmp cmp);
int borrarLruCache(tCache *cache, const void *clave, Cmp cmp);
void vaciarLruCache(tCache *cache);

#endif // TDA_CACHE_LISTA_H_INCLUDED
