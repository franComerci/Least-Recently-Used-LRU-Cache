#include "TDA_LISTA_DOBLE.h"
#include "../TDA_ARBOL/TDA_ARBOL.h"

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
