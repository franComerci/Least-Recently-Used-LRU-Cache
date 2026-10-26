#include "TDA_ARBOL.h"
void crearArbol(tArbol *pa)
{
    *pa = NULL;
}

int insertarOrdenado(tArbol *pa, void *dato, unsigned tam, Cmp comparar)
{
    int cmp;
    tNodo *nue;

    if (!*pa)
    {
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
        nue->izq = NULL;
        nue->der = NULL;
        *pa = nue;
        return 0;
    }

    cmp = comparar(dato, (*pa)->dato);

    if (cmp > 0)
        return insertarOrdenado(&(*pa)->der, dato, tam, comparar);
    if (cmp < 0)
        return insertarOrdenado(&(*pa)->izq, dato, tam, comparar);

    return 1;                                   // duplicado
}

tArbol *buscarClave(tArbol *pa, Cmp comparar, void *clave)
{
    int cmp;

    if (!*pa)
        return NULL;

    cmp = comparar(clave, (*pa)->dato);

    if (cmp < 0)
        return buscarClave(&(*pa)->izq, comparar, clave);
    if (cmp > 0)
        return buscarClave(&(*pa)->der, comparar, clave);

    return pa;
}

void intercambio(tArbol *elim, tArbol *paR)
{
    void *auxDato = (*elim)->dato;
    unsigned auxTam = (*elim)->tam;

    (*elim)->dato = (*paR)->dato;
    (*elim)->tam  = (*paR)->tam;

    (*paR)->dato = auxDato;
    (*paR)->tam  = auxTam;
}

int eliminarNodo(tArbol *pa, void *dato, unsigned tam, Cmp comparar)
{
    tArbol *elim, *paR;
    tNodo *nodoAEliminar;
    int izq, der;

    elim = buscarClave(pa, comparar, dato);
    if (!elim)
        return 1;                               // no existe

    // hoja
    if (!(*elim)->izq && !(*elim)->der)
    {
        free((*elim)->dato);
        free(*elim);
        *elim = NULL;
        return 0;
    }

    // elijo el reemplazo del lado más alto
    izq = alturaArbol(&(*elim)->izq);
    der = alturaArbol(&(*elim)->der);

    if (izq < der)
    {
        paR = &(*elim)->der;                    // menor del subárbol derecho
        while ((*paR)->izq)
            paR = &(*paR)->izq;
    }
    else
    {
        paR = &(*elim)->izq;                    // mayor del subárbol izquierdo
        while ((*paR)->der)
            paR = &(*paR)->der;
    }

    nodoAEliminar = *paR;
    intercambio(elim, paR);                     // el dato a borrar queda en nodoAEliminar

    // el reemplazo tiene a lo sumo un hijo
    if (nodoAEliminar->der)
        *paR = nodoAEliminar->der;
    else
        *paR = nodoAEliminar->izq;              // puede ser NULL

    free(nodoAEliminar->dato);
    free(nodoAEliminar);
    return 0;
}

int arbolVacio(const tArbol *pa)
{
    return *pa == NULL;
}

int arbolLleno(const tArbol *pa, unsigned tam)
{
    void *nodo = malloc(sizeof(tNodo));
    void *dato = malloc(tam);
    int lleno = (nodo == NULL || dato == NULL);

    free(nodo);
    free(dato);
    return lleno;
}

void vaciarArbol(tArbol *pa)
{
    if (!*pa)
        return;

    vaciarArbol(&(*pa)->izq);
    vaciarArbol(&(*pa)->der);
    free((*pa)->dato);
    free(*pa);
    *pa = NULL;
}

int contarNodos(const tArbol *pa)
{
    if (!*pa)
        return 0;

    return 1 + contarNodos(&(*pa)->izq) + contarNodos(&(*pa)->der);
}

int alturaArbol(const tArbol *pa)
{
    int izq, der;

    if (!*pa)
        return 0;

    izq = alturaArbol(&(*pa)->izq);
    der = alturaArbol(&(*pa)->der);

    return 1 + MAXIMO(izq, der);
}

void recorrerPreorden(const tArbol *pa, tAccion accion)
{
    if (!*pa)
        return;

    accion((*pa)->dato, NULL);
    recorrerPreorden(&(*pa)->izq, accion);
    recorrerPreorden(&(*pa)->der, accion);
}

int compararCaracteres(const void *a, const void *b)
{
    char a1 = *(const char *)a;
    char b1 = *(const char *)b;
    return a1 - b1;
}

void mostrarLista(void *a, void *b)
{
    printf("%c\n", *(char *)a);
}