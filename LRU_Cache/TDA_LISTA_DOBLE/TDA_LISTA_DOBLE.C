#include "TDA_LISTA_DOBLE.h"
#include "../TDA_ARBOL/TDA_ARBOL.h"
///FUNCIONES DE CACHE CON LISTA DOBLE

void crearLruCacheListaDobleyArbol(tCacheListaDoble *cache, size_t maxCap, size_t tamClave, size_t tamDato, Cmp cmp)
{
    cache->cap = maxCap;
    cache->tamClave = tamClave;
    cache->tamDato = tamDato;
    cache->cmp = cmp;
    crearListaDoble(&cache->listaDoble);
    crearArbol(&cache->arbol);
    cache->ce = 0;
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


///PRIMITIVAS LISTA DOBLE

void crearListaDoble(tListaDoble *pl)
{
    *pl = NULL;
}

int insertarOrdenadoDoble(tListaDoble *pl, const void *dato, unsigned tam, Cmp comparar)
{
    tNodo2 *nue, *act;
    tNodo2 *auxAnt = NULL, *auxSig = NULL;
    int cmp;

    if (*pl)
    {
        act = *pl;

        while (act->ant && comparar(act->dato, dato) > 0)
            act = act->ant;

        while (act->sig && comparar(act->dato, dato) < 0)
            act = act->sig;

        cmp = comparar(act->dato, dato);

        if (cmp == 0)
            return 1;                   // duplicado

        if (cmp < 0)                    // va después de act
        {
            auxAnt = act;
            auxSig = act->sig;
        }
        else                            // va antes de act
        {
            auxAnt = act->ant;
            auxSig = act;
        }
    }

    nue = (tNodo2 *)malloc(sizeof(tNodo2));
    if (!nue)
        return 1;

    nue->dato = malloc(tam);
    if (!nue->dato)
    {
        free(nue);
        return 1;
    }

    memcpy(nue->dato, dato, tam);
    nue->tam = tam;
    nue->sig = auxSig;
    nue->ant = auxAnt;

    if (auxSig)
        auxSig->ant = nue;
    if (auxAnt)
        auxAnt->sig = nue;

    *pl = nue;
    return 0;
}

int eliminarNodoDoble(tListaDoble *pl, void *dato, unsigned tam, Cmp comparar)
{
    tNodo2 *elim, *auxAnt, *auxSig;

    if (!*pl)
        return 1;

    elim = *pl;
    while (elim->ant)                               // voy al principio
        elim = elim->ant;

    while (elim && comparar(elim->dato, dato) != 0) // busco la clave
        elim = elim->sig;

    if (!elim)
        return 1;                                   // no existe

    auxAnt = elim->ant;
    auxSig = elim->sig;

    if (auxAnt)
        auxAnt->sig = auxSig;
    if (auxSig)
        auxSig->ant = auxAnt;

    *pl = auxSig ? auxSig : auxAnt;                 // queda en un nodo válido o NULL

    free(elim->dato);
    free(elim);
    return 0;
}

int listaLlenaDoble(const tListaDoble *pl, unsigned tam)
{
    void *nodo = malloc(sizeof(tNodo2));
    void *dato = malloc(tam);
    int llena = (nodo == NULL || dato == NULL);

    free(nodo);
    free(dato);
    return llena;
}

int listaVaciaDoble(const tListaDoble *pl)
{
    return *pl == NULL;
}

void vaciarListaDoble(tListaDoble *pl)
{
    tNodo2 *elim;

    if (!*pl)
        return;

    while ((*pl)->ant)                              // voy al principio
        *pl = (*pl)->ant;

    while (*pl)
    {
        elim = *pl;
        *pl = elim->sig;
        free(elim->dato);
        free(elim);
    }
}
