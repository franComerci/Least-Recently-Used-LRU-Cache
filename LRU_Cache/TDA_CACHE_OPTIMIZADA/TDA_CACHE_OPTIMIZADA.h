#ifndef TDA_CACHE_OPTIMIZADA_H_INCLUDED
#define TDA_CACHE_OPTIMIZADA_H_INCLUDED

#include "../TDA_LISTA_DOBLE/TDA_LISTA_DOBLE.h"
#include "../TDA_ARBOL/TDA_ARBOL.h"
#include "../TDA_Cache_Lista_Simple/TDA_Cache_Lista.h"
typedef struct
{
    void *clave;
    tNodo2 *nodoLista;
    Cmp cmp;
} tIndice;

/*
Es para buscar adentro del arbol pero con una funcion que cmp que no tenga
que saber como es la estrcutura del nodo de la lista doble
*/

typedef struct
{
    tListaDoble listaDoble;
    tArbol arbol;
    tNodo2 *ultNodo;
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

///FUNCIONES DE CACHE CON LISTA DOBLE

void crearLruCacheListaDobleyArbol(tCacheListaDoble *cache, size_t maxCap, size_t tamClave, size_t tamDato, Cmp cmp);
int agregarLruCacheListaDobleyArbol(tCacheListaDoble *cache, const void *clave, const void *dato);
int obtenerLruCacheListaDobleyArbol(tCacheListaDoble *cache, const void *clave, void *dato);
int borrarLruCacheListaDobleyArbol(tCacheListaDoble *cache, const void *clave);
int vaciarLruCacheListaDobleyArbol(tCacheListaDoble *cache);
int destruirLruCacheListaDobleyArbol(tCacheListaDoble *cache);
int cacheListaDobleLlenayArbol(const tCacheListaDoble *cache);
int cacheListaDobleVaciayArbol(const tCacheListaDoble *cache);
int alturaIndiceLruCacheListaDobleyArbol(const tCacheListaDoble *cache);


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

#endif // TDA_CACHE_OPTIMIZADA_H_INCLUDED
