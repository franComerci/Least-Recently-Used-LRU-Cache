#ifndef TDA_CACHE_OPTIMIZADA_H_INCLUDED
#define TDA_CACHE_OPTIMIZADA_H_INCLUDED

#include "../TDA_LISTA_DOBLE/TDA_LISTA_DOBLE.h"
#include "../TDA_ARBOL/TDA_ARBOL.h"
void crearLruCacheListaDobleyArbol(tCacheListaDoble *cache, size_t maxCap, size_t tamClave, size_t tamDato, Cmp cmp);
int agregarLruCacheListaDobleyArbol(tCacheListaDoble *cache, const void *clave, const void *dato);
int obtenerLruCacheListaDobleyArbol(tCacheListaDoble *cache, const void *clave, void *dato);
int borrarLruCacheListaDobleyArbol(tCacheListaDoble *cache, const void *clave);
int vaciarLruCacheListaDobleyArbol(tCacheListaDoble *cache);
int destruirLruCacheListaDobleyArbol(tCacheListaDoble *cache);
int cacheListaDobleLlenayArbol(const tCacheListaDoble *cache);
int cacheListaDobleVaciayArbol(const tCacheListaDoble *cache);

#endif // TDA_CACHE_OPTIMIZADA_H_INCLUDED
