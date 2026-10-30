#include "TDA_CACHE_OPTIMIZADA.h"

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
    tElemCacheListaDoble aux;
    aux.clave = (void *)clave;//ya tengo la clave del nodo

    tNodo2 nodoAux;
    nodoAux.dato = &aux;
    tNodo2 *ptrNodo = &nodoAux;//puntero q apunta a la clave

    tArbol *nodoArbol = buscarClave(&cache->arbol, cache->cmp, &ptrNodo);//me devuelve el nodo en el arbol
    //veo si el nodo ya existe
    if(nodoArbol && *nodoArbol)
    {
        tNodo2 *nodoLista;
        memcpy(&nodoLista, (*nodoArbol)->dato, sizeof(tNodo2 *));

        tElemCacheListaDoble *elem = (tElemCacheListaDoble *)nodoLista->dato;
        memcpy(elem->dato, dato, cache->tamDato);//actializo dato con el que me llega por parametro

        //la consigna me dice que lo tengo q traer al principio
        //la consigna me dice que lo tengo q traer al principio
        if(cache->listaDoble != nodoLista)
        {
            // 1. Si era el último, actualizamos la cola
            if(cache->ultNodo == nodoLista)
                cache->ultNodo = nodoLista->ant;

            // 2. Conectamos a los vecinos entre sí para cerrar el hueco
            if(nodoLista->ant)
                nodoLista->ant->sig = nodoLista->sig;
            if(nodoLista->sig)
                nodoLista->sig->ant = nodoLista->ant;

            // 3. Lo llevamos al principio (esto ya lo tenías bien)
            nodoLista->sig = cache->listaDoble;
            nodoLista->ant = NULL;

            if(cache->listaDoble)
                cache->listaDoble->ant = nodoLista;

            cache->listaDoble = nodoLista;
        }
        return 1;
    }

    //veo si esta llena la cache
    if(cache->ce == cache->cap)
    {
        tNodo2 *ultimo = cache->ultNodo;//arranco desde el principio

        if(ultimo)
        {
//            while(ultimo->sig)//recorro hasta que sig sea distinto de NULL
//                ultimo = ultimo->sig;

            tElemCacheListaDoble *elemUlt = (tElemCacheListaDoble *)ultimo->dato;

            tElemCacheListaDoble auxBorrar; //tengo que borrar el ultimo por la consigna
            auxBorrar.clave = elemUlt->clave;
            tNodo2 nodoBorrar;
            nodoBorrar.dato = &auxBorrar;

            tNodo2 *ptrBorrar = &nodoBorrar;

            eliminarNodo(&cache->arbol, &ptrBorrar, sizeof(tNodo2 *), cache->cmp);

            cache->ultNodo = ultimo->ant;

            if(ultimo->ant)//conecto el ultimo
                ultimo->ant->sig = NULL;
            else//si ant es null, significa q la lista está vacía
                cache->listaDoble = NULL;

            free(elemUlt->clave);
            free(elemUlt->dato);
            free(ultimo->dato);
            free(ultimo);

            cache->ce--;
        }
    }
    //ahora tengo que meter el nuevo dato
    tNodo2 *nuevo = (tNodo2 *)malloc(sizeof(tNodo2));
    if(!nuevo)
        return 0;

    tElemCacheListaDoble *elemNuevo = (tElemCacheListaDoble *)malloc(sizeof(tElemCacheListaDoble));

    if(!elemNuevo)
    {
        free(nuevo);
        return 0;
    }

    elemNuevo->clave = malloc(cache->tamClave);
    elemNuevo->dato = malloc(cache->tamDato);

    memcpy(elemNuevo->clave, clave, cache->tamClave);
    memcpy(elemNuevo->dato, dato, cache->tamDato);

    nuevo->dato = elemNuevo;
    nuevo->tam = sizeof(tElemCacheListaDoble);
    //el nuevo nodo tiene que ser el primero porq es el ultimo que entro
    nuevo->ant = NULL;
    nuevo->sig = cache->listaDoble;

    if(cache->listaDoble)
        cache->listaDoble->ant = nuevo;
    else
        cache->ultNodo = nuevo;

    cache->listaDoble = nuevo;
    //lo tengo que guardar en el arbol
    insertarOrdenado(&cache->arbol, &nuevo, sizeof(tNodo2 *), cache->cmp);

    cache->ce++;

    return 1;
}


int obtenerLruCacheListaDobleyArbol(tCacheListaDoble *cache, const void *clave, void *dato)
{
    tElemCacheListaDoble aux;
    aux.clave = (void *)clave;//ya tengo la clave del nodo

    tNodo2 nodoAux;
    nodoAux.dato = &aux;
    tNodo2 *ptrNodo = &nodoAux;//puntero q apunta a la clave

    tArbol *nodoArbol = buscarClave(&cache->arbol, cache->cmp, &ptrNodo);//me devuelve el nodo en el arbol

    if(nodoArbol && *nodoArbol)
    {
        tNodo2 *nodoLista;
        memcpy(&nodoLista, (*nodoArbol)->dato, sizeof(tNodo2 *));

        if(cache->listaDoble != nodoLista)//distintas posiciones en memoria
        {
            if(cache->ultNodo == nodoLista)
                cache->ultNodo = nodoLista->ant;

            if(nodoLista->ant)//si existe un nodo antes tengo que enlazarlo con el siguiente, porque saco al del medio
                nodoLista->ant->sig = nodoLista->sig;
            if(nodoLista->sig)//lo mismo que antes pero al reves
                nodoLista->sig->ant = nodoLista->ant;

            nodoLista->sig = cache->listaDoble;//hago que el que se obtiene ahora apunte al inicio de la cache
            nodoLista->ant = NULL;

            if(cache->listaDoble)//no esta vacia
                cache->listaDoble->ant = nodoLista;//muevo a segunda posicion

            cache->listaDoble = nodoLista;//ahora es el inicio de la lista
        }

        tElemCacheListaDoble *elem = (tElemCacheListaDoble *)nodoLista->dato;
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

    cache->ce = 0;

    return 1;
}
///BORRAR CACHE Y LISTA DOBLE

int borrarLruCacheListaDobleyArbol(tCacheListaDoble *cache, const void *clave){
    tElemCacheListaDoble elemAux;
    elemAux.clave = (void*) clave;

    tNodo2 nodoAux;
    nodoAux.dato = &elemAux;
    tNodo2 *ptrNodo = &nodoAux; // tNodo2 *

    tArbol *pSubArbol = buscarClave(&cache->arbol, cache->cmp,&ptrNodo); //aca pasamos ptrNodo como tNodo **, que es el formato que tiene el arbol
    if(!pSubArbol || !*pSubArbol)// si el arbol no tiene elementos O no se encontro el elemento
        return 0;//no encontro

    tNodo2 * nodoLista = *(tNodo2**)(*pSubArbol)->dato; //obtenemos el puntero al nodo de la lista guardado en el arbol
    // (*pSubArbol)->dato aca accedemos al campo dato de ese nodo del arbol
    // Luego necesitamos castear para que el programa lo reinterprete (tNodo2 **) y con el * accedemos al dato


    if(nodoLista->ant)//ahora desenlazamos el nodo de la lista doble
        nodoLista->ant->sig = nodoLista->sig;
    else
        cache->listaDoble = nodoLista->sig;

    if(nodoLista->sig)
        nodoLista->sig->ant = nodoLista->ant;

    //eliminamos del arbol
    eliminarNodo(&cache->arbol,&ptrNodo,sizeof(tNodo2 *),cache->cmp);

    //eliminamos el nodo de la lista y la memoria
    tElemCacheListaDoble *elem = (tElemCacheListaDoble*) nodoLista->dato;
    if(elem){
        free(elem->clave);
        free(elem->dato);
        free(elem);
    }

    if(cache->ultNodo == nodoLista)
        cache->ultNodo = nodoLista->ant;
    free(nodoLista);

    cache->ce--;

    return 1;

}
