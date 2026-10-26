#ifndef TDA_CACHE_LISTA_H_INCLUDED
#define TDA_CACHE_LISTA_H_INCLUDED

#include "../TDA_LISTA/TDA_Lista.h"
#include "../comun.h"
typedef struct
{
    tLista lista;
    int cap;
    int ce;
    size_t tamClave;
    size_t tamDato;
    Cmp cmp;
}tCache;

typedef struct
{
    void *dato;//contenido
    void *clave;//para buscar
}tElemCache;//para guardar en la lista

typedef int (*Cmp)(const void *a, const void *b);
typedef void (*actualizar) (void*, const void*);

void crearLruCache(tCache *cache, size_t maxCap, size_t tamClave, size_t tamDato, Cmp cmp);
int agregarLruCache(tCache *cache, const void *clave, const void *dato);
int obtenerLruCache(tCache *cache, const void *clave, void *dato);
int borrarLruCache(tCache *cache, const void *clave);
void vaciarLruCache(tCache *cache);

int extraerDeCache(tCache *cache, const void *clave, tElemCache *elem);
#endif // TDA_CACHE_LISTA_H_INCLUDED
