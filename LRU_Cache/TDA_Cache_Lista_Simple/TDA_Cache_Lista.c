#include "TDA_Cache_Lista.h"

void crearLruCache(tCache *cache, size_t maxCap, size_t tamClave, size_t tamDato)
{
    crearLista(&(cache->lista));
    cache->cap = maxCap;
    cache->ce = 0;
    cache->tamClave = tamClave;
    cache->tamDato = tamDato;
}

int agregarLruCache(tCache *cache, const void *clave, const void *dato, Cmp cmp)
{
    tElemCache elem;

    if(extraerDeCache(cache, clave, &elem, cmp))//me fijo si esta en la cache
    {
        memcpy(elem.dato, dato, cache->tamDato);
    }
    else//no esta
    {
        if(cache->cap == cache->ce)//lleno
        {
            tElemCache elemAnterior;
            if(!sacarUltimoLista(&(cache->lista), &elemAnterior, sizeof(tElemCache)))//borro el ultimo
                return 0;

            free(elemAnterior.clave);
            free(elemAnterior.dato);//tengo que liberar la memoria del anterior
            cache->ce--;
        }

        elem.clave = malloc(cache->tamClave);//pido memoria para el actual
        elem.dato = malloc(cache->tamDato);

        memcpy(elem.clave, clave, cache->tamClave);
        memcpy(elem.dato, dato, cache->tamDato);

        cache->ce++;
    }
    //siempre se agrega al principio
    ponerPrimeroLista(&(cache->lista), &elem, sizeof(tElemCache));

    return 1;
}

int obtenerLruCache(tCache *cache, const void *clave, void *dato, Cmp cmp)
{
    tElemCache elem;

    if(extraerDeCache(cache, clave, &elem, cmp))
    {
        memcpy(dato, elem.dato, cache->tamDato);
        ponerPrimeroLista(&(cache->lista), &elem, sizeof(tElemCache));
        return 1;
    }

    return 0;
}

int borrarLruCache(tCache *cache, void *clave, Cmp cmp)
{
    tElemCache elem;

    if(extraerDeCache(cache, clave, &elem, cmp))
    {
        free(elem.clave);
        free(elem.dato);
        cache->ce--;
        return 1;
    }

    return 0;//no existe
}

void vaciarLruCache(tCache *cache)
{
    tElemCache elem;
    while(sacarPrimeroLista(&(cache->lista), &elem, sizeof(tElemCache)))
    {
        free(elem.clave);
        free(elem.dato);
    }


    cache->ce = 0;
}

int extraerDeCache(tCache *cache, const void *clave, tElemCache *elem, Cmp cmp)
{
    tNodo *actual = cache->lista;
    tNodo *anterior = NULL;

    while(actual)
    {
        tElemCache *elemActual = (tElemCache *)actual->info;

        if(cmp(elemActual->clave, clave) == 0)//encuentro la clave
        {
            *elem = *elemActual;
            //saco el nodo
            //esto es para conectar los nodos, sacando el que esta en el medio
            if(anterior == NULL)//primero
                cache->lista = actual->sig;
            else
                anterior->sig = actual->sig;

            free(actual->info);
            free(actual);

            return 1;
        }

        anterior = actual;
        actual = actual->sig;
    }
    return 0;
}
