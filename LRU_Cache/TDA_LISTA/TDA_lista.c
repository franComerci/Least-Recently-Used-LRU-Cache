#include "TDA_Lista.h"

void crearLista(tLista *pl)
{
    *pl = NULL;
}

int listaVacia(const tLista *pl)
{
    return *pl == NULL;
}

int listaLlena(const tLista *pl, size_t cantBytes)
{
    tNodo1 *aux = (tNodo1 *)malloc(sizeof(tNodo1));
    void *info = malloc(cantBytes);

    free(aux);
    free(info);

    return aux == NULL || info == NULL;//aux o info tendr�an NULL si nunca se pudo hacer el malloc
}

void vaciarLista(tLista *pl)
{
    while(*pl)
    {
        tNodo1 *aux = *pl;
        *pl = aux->sig;
        free(aux->info); //voy liberando al anterior de pl en escalerita
        free(aux);
    }
}

int ponerPrimeroLista(tLista *pl, const void *pd, size_t cantBytes)
{
    tNodo1 *nuevo;
    if((nuevo = (tNodo1 *)malloc(sizeof(tNodo1))) == NULL ||
       (nuevo->info = malloc(cantBytes)) == NULL)//valido que info y nuevo se carguen dinamicamente bien
    {
        free(nuevo);
        return 0;
    }

    memcpy(nuevo->info, pd, cantBytes);//copio elemento
    nuevo->tamInfo = cantBytes;//copio tama�o
    nuevo->sig = *pl;//el siguiente va a ser el actual inicio de la lista
    *pl = nuevo;//actualizo el pl

    return 1;
}

int sacarPrimeroLista(tLista *pl, void *pd, size_t cantBytes)
{
    tNodo1 *aux = *pl;

    if(!aux)
        return 0;

    *pl = aux->sig;
    memcpy(pd, aux->info, MIN(cantBytes, aux->tamInfo));//saco el min de tama�o porque puede ser que no este contiguo en memoria
    free(aux->info);
    free(aux);

    return 1;
}

int verPrimeroLista(tLista *pl, void *pd, size_t cantBytes)
{
    if(*pl == NULL)
        return 0;

    memcpy(pd, (*pl)->info, MIN( (*pl)->tamInfo, cantBytes));
    return 1;
}

int ponerUltimoLista(tLista *pl, const void *pd, size_t cantBytes)
{
    tNodo1 *nuevo;

    while(*pl)
        pl = &(*pl)->sig;//recorro hasta que llego a NULL

    if((nuevo = (tNodo1 *)malloc(sizeof(tNodo1))) == NULL ||
       (nuevo->info = malloc(cantBytes)) == NULL)//valido que info y nuevo se carguen dinamicamente bien
    {
        free(nuevo);
        return 0;
    }

    memcpy(nuevo->info, pd, cantBytes);
    nuevo->tamInfo = cantBytes;
    nuevo->sig = NULL;//el siguiente al actual pasa a ser NULL
    *pl = nuevo;

    return 1;
}

int sacarUltimoLista(tLista *pl, void *pd, size_t cantBytes)
{
    if(*pl == NULL)
        return 0;

    while((*pl)->sig)
        pl = &(*pl)->sig;//recorro hasta que llego a NULL

    memcpy(pd, (*pl)->info, cantBytes);
    free((*pl)->info);
    free((*pl));
    *pl = NULL;

    return 1;
}

int verUltimoLista(tLista *pl, void *pd, size_t cantBytes)
{
    if(*pl == NULL)
        return 0;

    while((*pl)->sig)
        pl = &(*pl)->sig;//recorro hasta que llego a NULL

    memcpy(pd, (*pl)->info, cantBytes);

    return 1;
}
