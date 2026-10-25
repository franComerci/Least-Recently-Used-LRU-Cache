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
    void *dato;//contenido
    void *clave;//para buscar
}tElemCache;//para guardar en la lista

typedef int (*Cmp)(const void *a, const void *b);


void crearLruCache(tCache *cache, size_t maxCap, size_t tamClave, size_t tamDato, Cmp cmp);
int agregarLruCache(tCache *cache, const void *clave, const void *dato, Cmp cmp);
int obtenerLruCache(tCache *cache, const void *clave, void *dato, Cmp cmp);
int borrarLruCache(tCache *cache, void *clave, Cmp cmp);
void vaciarLruCache(tCache *cache);

int extraerDeCache(tCache *cache, const void *clave, tElemCache *elem, Cmp cmp);
#endif // TDA_CACHE_LISTA_H_INCLUDED
