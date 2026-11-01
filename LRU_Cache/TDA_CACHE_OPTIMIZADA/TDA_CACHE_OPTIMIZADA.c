#include "TDA_CACHE_OPTIMIZADA.h"

static int cmpIndices(const void *a, const void *b)
{
    const tIndice *indA = (const tIndice *)a;
    const tIndice *indB = (const tIndice *)b;

    return indA->cmp(indA->clave, indB->clave); //llama a la funcion del usuario
}
/* con esto en la funcion de cmp solo paso como tienen que ser las claves 
y no me tengo que preocupar por la estructura del nodo de la lista doble, porque aca ya lo hago yo
*/
///FUNCIONES DE CACHE CON LISTA DOBLE Y ARBOL

void crearLruCacheListaDobleyArbol(tCacheListaDoble *cache, size_t maxCap, size_t tamClave, size_t tamDato, Cmp cmp)
{
    cache->cap = maxCap;
    cache->tamClave = tamClave;
    cache->tamDato = tamDato;
    cache->cmp = cmp;
    crearListaDoble(&cache->listaDoble);
    crearArbol(&cache->arbol);
    cache->ultNodo = NULL;
    cache->ce = 0;
}

int agregarLruCacheListaDobleyArbol(tCacheListaDoble *cache, const void *clave, const void *dato)
{
    tIndice indBusq;
    tArbol *nodoArbol;
    tElemCacheListaDoble *elemNuevo;
    tIndice indNuevo;

    indBusq.clave = (void *)clave; //ya tengo la clave del nodo
    indBusq.nodoLista = NULL;
    indBusq.cmp = cache->cmp;

    /* 1. Verificamos si la clave ya existe en la cache */
    nodoArbol = buscarClave(&cache->arbol, cmpIndices, &indBusq); //me devuelve el nodo en el arbol
    //veo si el nodo ya existe
    if(nodoArbol && *nodoArbol)
    {
        tIndice *indEncontrado = (tIndice *)(*nodoArbol)->dato;
        tNodo2 *nodoLista = indEncontrado->nodoLista;
        tElemCacheListaDoble *elem = (tElemCacheListaDoble *)nodoLista->dato;

        /* actualizo el dato con el que me llega por parametro */
        memcpy(elem->dato, dato, cache->tamDato);

        /* la consigna me dice que lo tengo que traer al principio */
        if(cache->listaDoble != nodoLista)
        {
            /* 1. Si era el ultimo, actualizamos la cola */
            if(cache->ultNodo == nodoLista)
                cache->ultNodo = nodoLista->ant;

            /* 2. Conectamos a los vecinos entre si para cerrar el hueco*/
            if(nodoLista->ant)
                nodoLista->ant->sig = nodoLista->sig;
            if(nodoLista->sig)
                nodoLista->sig->ant = nodoLista->ant;

            /* 3. Lo llevamos al principio */
            nodoLista->sig = cache->listaDoble;
            nodoLista->ant = NULL;

            if(cache->listaDoble)
                cache->listaDoble->ant = nodoLista;

            cache->listaDoble = nodoLista;
        }
        return 1;
    }

    /* 2. Si la cache esta llena, quito el ultimo */
    if(cache->ce == cache->cap)
    {
        tNodo2 *ultimo = cache->ultNodo;

        if(ultimo)
        {
            tElemCacheListaDoble *elemUlt = (tElemCacheListaDoble *)ultimo->dato;
            tIndice indBorrar;

            indBorrar.clave = elemUlt->clave;
            indBorrar.nodoLista = NULL;
            indBorrar.cmp = cache->cmp;

            /* Borramos el indice del arbol */
            eliminarNodo(&cache->arbol, &indBorrar, sizeof(tIndice), cmpIndices);

            /* Actualizamos la cola de la lista */
            cache->ultNodo = ultimo->ant;

            if(ultimo->ant) /* conecto el ultimo*/
                ultimo->ant->sig = NULL;
            else /*si ant es null, significa que la lista esta vacia*/
                cache->listaDoble = NULL;

            free(elemUlt->clave);
            free(elemUlt->dato);
            free(ultimo->dato);
            free(ultimo);

            cache->ce--;
        }
    }

    /* 3. Insertamos el nuevo dato al inicio */

    indNuevo.nodoLista = (tNodo2 *)malloc(sizeof(tNodo2));
    if (!indNuevo.nodoLista)
        return 0;

    elemNuevo = (tElemCacheListaDoble *)malloc(sizeof(tElemCacheListaDoble));
    if (!elemNuevo)
    {
        free(indNuevo.nodoLista);
        return 0;
    }

    elemNuevo->clave = malloc(cache->tamClave);
    if (!elemNuevo->clave)
    {
        free(indNuevo.nodoLista);
        free(elemNuevo);
        return 0;
    }

    elemNuevo->dato = malloc(cache->tamDato);
    if (!elemNuevo->dato)
    {
        free(indNuevo.nodoLista);
        free(elemNuevo->clave);
        free(elemNuevo);
        return 0;
    }

    memcpy(elemNuevo->clave, clave, cache->tamClave);
    memcpy(elemNuevo->dato, dato, cache->tamDato);

    indNuevo.nodoLista->dato = elemNuevo;
    indNuevo.nodoLista->tam = sizeof(tElemCacheListaDoble);
    /* el nodo nuevo tiene que ser el primero porque es el ultimo que entro*/
    indNuevo.nodoLista->ant = NULL;
    indNuevo.nodoLista->sig = cache->listaDoble;

    if (cache->listaDoble)
        cache->listaDoble->ant = indNuevo.nodoLista;
    else
        cache->ultNodo = indNuevo.nodoLista;

    cache->listaDoble = indNuevo.nodoLista;

    /* Guardo el indice en el arbol */
    indNuevo.clave = elemNuevo->clave;
    indNuevo.cmp = cache->cmp;

    insertarOrdenado(&cache->arbol, &indNuevo, sizeof(tIndice), cmpIndices);

    cache->ce++;

    return 1;
}

int obtenerLruCacheListaDobleyArbol(tCacheListaDoble *cache, const void *clave, void *dato)
{
    tIndice indBusq;
    tArbol *nodoArbol;

    if(!cache)
        return 0;

    indBusq.clave = (void *)clave; /* Ya tengo la clave del nodo*/
    indBusq.nodoLista = NULL;
    indBusq.cmp = cache->cmp;

    nodoArbol = buscarClave(&cache->arbol, cmpIndices, &indBusq);

    if(nodoArbol && *nodoArbol)
    {
        tIndice *indEncontrado = (tIndice *)(*nodoArbol)->dato;
        tNodo2 *nodoLista = indEncontrado->nodoLista;
        tElemCacheListaDoble *elem;

        /* Si no esta en la cabeza, lo muevo al principio */
        if(cache->listaDoble != nodoLista)
        {
            if(cache->ultNodo == nodoLista)
                cache->ultNodo = nodoLista->ant;

            if(nodoLista->ant) /* Si existe un nodo antes tnego que enlazarlo con el siguiente, porque saco al del medio*/
                nodoLista->ant->sig = nodoLista->sig;
            if(nodoLista->sig) /* lo mismo que antes pero al reves*/
                nodoLista->sig->ant = nodoLista->ant;

            nodoLista->sig = cache->listaDoble; /* hago que el que se obtiene ahora apunte al inicio de la cache*/
            nodoLista->ant = NULL;

            if(cache->listaDoble) /* no esta vacia */
                cache->listaDoble->ant = nodoLista;/* muevo a segunda posicion*/

            cache->listaDoble = nodoLista; /* ahora es el inicio de la lista*/
        }

        elem = (tElemCacheListaDoble *)nodoLista->dato;
        memcpy(dato, elem->dato, cache->tamDato);
        return 1;
    }

    return 0;
}

int vaciarLruCacheListaDobleyArbol(tCacheListaDoble *cache)
{
    tNodo2 *actual = cache->listaDoble;

    while(actual)
    {
        tElemCacheListaDoble *elem = (tElemCacheListaDoble *)actual->dato;
        free(elem->clave);
        free(elem->dato);
        actual = actual->sig;
    }
    vaciarListaDoble(&cache->listaDoble);
    vaciarArbol(&cache->arbol);

    cache->ultNodo = NULL;
    cache->ce = 0;

    return 1;
}
///BORRAR CACHE Y LISTA DOBLE

int borrarLruCacheListaDobleyArbol(tCacheListaDoble *cache, const void *clave)
{
    tIndice indBorrar;
    tArbol *pSubArbol;
    tIndice *indEncontrado;
    tNodo2 *nodoLista;
    tElemCacheListaDoble *elem;

    if(!cache)
        return 0;

    indBorrar.clave = (void *)clave;
    indBorrar.nodoLista = NULL;
    indBorrar.cmp = cache->cmp;

    pSubArbol = buscarClave(&cache->arbol, cmpIndices, &indBorrar);
    if(!pSubArbol || !*pSubArbol) /* si el arbol no tiene elementos o no se encontro el elemento*/
        return 0; /* No encontre la clave */

    indEncontrado = (tIndice *)(*pSubArbol)->dato;
    nodoLista = indEncontrado->nodoLista; /* obtenemos el puntero al nodo de la lista guardado en el arbol*/

    /* 1. Desenlazo el nodo de la lista doble */
    if(nodoLista->ant)
        nodoLista->ant->sig = nodoLista->sig;
    else
        cache->listaDoble = nodoLista->sig;

    if(nodoLista->sig)
        nodoLista->sig->ant = nodoLista->ant;

    if(cache->ultNodo == nodoLista)
        cache->ultNodo = nodoLista->ant;

    /* 2. Elimino del arbol */
    eliminarNodo(&cache->arbol, &indBorrar, sizeof(tIndice), cmpIndices);

    /* 3. Elimino el nodo de la lista y sus datos */
    elem = (tElemCacheListaDoble *)nodoLista->dato;
    if(elem)
    {
        free(elem->clave);
        free(elem->dato);
        free(elem);
    }
    free(nodoLista);

    cache->ce--;
    return 1;
}

int alturaIndiceLruCacheListaDobleyArbol(const tCacheListaDoble *cache)
{
    if(!cache)
        return 0;

    return alturaArbol(&cache->arbol);
}