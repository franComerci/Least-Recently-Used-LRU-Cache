#ifndef FUNCIONESDEFENSA_H_INCLUDED
#define FUNCIONESDEFENSA_H_INCLUDED
#include "../TDA_ARBOL/TDA_ARBOL.h"
#include "../TDA_Cache_Lista_Simple/TDA_Cache_Lista.h"
#include "../TDA_CACHE_OPTIMIZADA/TDA_CACHE_OPTIMIZADA.h"
#include "../TDA_LISTA_DOBLE/TDA_LISTA_DOBLE.h"
#include "../Utilitiarias.h"
#include "../comun.h"
#include "../Tweener/Tweener.h"
///FUNCIONES PARA LA DEFENSA
int consultarPeekLruCache(tCacheListaDoble *cache, const void *clave, void *dato);
int redimensionarLruCache(tCacheListaDoble *cache, size_t nuevaCapacidad);
void imprimirEstadoCache(const tCacheListaDoble *cache);
int contarHojasArbolIndices(const tArbol *arbol);
int obtenerTopNRecientes(const tCacheListaDoble *cache, int n, void *vectorSalida);
int obtenerTopNRecientesLista(const tCacheListaDoble *cache, int n, tNodo1 **lista);
int contarNodosArbol(const tArbol *arbol);
int validarConsistenciaCache(const tCacheListaDoble *cache);
void invertirOrdenCache(tCacheListaDoble *cache);
int obtenerTopNUltimosLista(const tCacheListaDoble *cache, int n, tNodo1 **lista);
int cacheFiltro(tCacheListaDoble *cache, int (*filtro)(const void *dato));
int mapCache(tCacheListaDoble *cache, int (*criterio)(const void *dato));
int extraerRangoClaves(tCacheListaDoble *cache, void *claveMin, void *claveMax, tListaDoble *listaSalida);
#endif // FUNCIONESDEFENSA_H_INCLUDED
