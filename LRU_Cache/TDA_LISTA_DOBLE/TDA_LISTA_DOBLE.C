#include "TDA_LISTA_DOBLE.h"
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

int agregarLruCacheListaDobleyArbol(tCacheListaDoble *cache, const void *clave, const void *dato)
{
    if (cache->ce >= cache->cap)
    {
        // Eliminar el elemento menos recientemente usado (LRU)
        tElemCacheListaDoble lru;
        if (obtenerLruCacheListaDoble(cache, NULL, &lru) == 0)
        {
            eliminarLruCacheListaDoble(cache, lru.clave);
        
            cache->ce--;
        }
    }

    // Insertar el nuevo elemento en la lista
    if (insertarOrdenadoDoble(&cache->listaDoble, dato, sizeof(tElemCacheListaDoble), cache->cmp) == 0 && insertarordenadoArbol(&cache->arbol, clave, dato, cache->tamClave, cache->tamDato, cache->cmp) == 0)
    {
        cache->ce++;
        return 0; // Éxito
    }
    return 1; // Error al insertar
}

int obtenerLruCacheListaDobleyArbol(tCacheListaDoble *cache, const void *clave, void *dato)
{
    
    tNodo *nodo = cache->listaDoble;
    // Buscar el nodo con la clave especificada
    while (nodo)
    {
        tElemCacheListaDoble *elem = (tElemCacheListaDoble *)nodo->dato;
        if (cache->cmp(elem->clave, clave) == 0)
        {
            // Copiar el dato al puntero proporcionado
            memcpy(dato, elem, sizeof(tElemCacheListaDoble));
            return 0; // Éxito
        }
        nodo = nodo->sig;
    }
    return 1; // No se encontró la clave
}

int borrarLruCacheListaDobleyArbol(tCacheListaDoble *cache, const void *clave)
{
    tNodo *nodo = cache->listaDoble;

    // Buscar el nodo con la clave especificada
    while (nodo)
    {
        tElemCacheListaDoble *elem = (tElemCacheListaDoble *)nodo->dato;
        if (cache->cmp(elem->clave, clave) == 0)
        {
            // Eliminar el nodo de la lista
            eliminarNodoDoble(&cache->listaDoble, elem, sizeof(tElemCacheListaDoble), cache->cmp);
            cache->ce--;
            return 0; // Éxito
        }
        nodo = nodo->sig;
    }
    return 1; // No se encontró la clave
}



///PRIMITIVAS LISTA DOBLE

void crearListaDoble(tListaDoble *pl)
{
    *pl = NULL;
}

int insertarOrdenadoDoble(tListaDoble *pl, const void *dato, unsigned tam, Cmp comparar)
{
    tNodo *nue, *act;
    tNodo *auxAnt = NULL, *auxSig = NULL;
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

    nue = (tNodo *)malloc(sizeof(tNodo));
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
    tNodo *elim, *auxAnt, *auxSig;

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
    void *nodo = malloc(sizeof(tNodo));
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
    tNodo *elim;

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