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

tArbol *buscarNodoEnArbol(tCacheListaDoble *cache, const void *clave)
{
    tIndice indBusq;
    if(!cache)
        return NULL;

    indBusq.clave = (void *)clave;
    indBusq.nodoLista = NULL;
    indBusq.cmp = cache->cmp;

    return buscarClave(&cache->arbol, cmpIndices, &indBusq);
}


//int agregarLruCacheListaDobleyArbol(tCacheListaDoble *cache, const void *clave, const void *dato)
//{
//    //buscarNodoEnArbol
//    tIndice indBusq;
//    tArbol *nodoArbol;
//    tElemCacheListaDoble *elemNuevo;
//    tIndice indNuevo;
//
//    indBusq.clave = (void *)clave; //ya tengo la clave del nodo
//    indBusq.nodoLista = NULL;
//    indBusq.cmp = cache->cmp;
//
//    /* 1. Verificamos si la clave ya existe en la cache */
//    nodoArbol = buscarClave(&cache->arbol, cmpIndices, &indBusq); //me devuelve el nodo en el arbol
//    //buscarNodoEnArbol
//    //veo si el nodo ya existe
//    if(nodoArbol && *nodoArbol)
//    {
//        tIndice *indEncontrado = (tIndice *)(*nodoArbol)->dato;
//        tNodo2 *nodoLista = indEncontrado->nodoLista;
//        tElemCacheListaDoble *elem = (tElemCacheListaDoble *)nodoLista->dato;
//
//        /* actualizo el dato con el que me llega por parametro */
//        memcpy(elem->dato, dato, cache->tamDato);
//
//        /* la consigna me dice que lo tengo que traer al principio */
//        if(cache->listaDoble != nodoLista)
//        {
//            /* 1. Si era el ultimo, actualizamos la cola */
//            if(cache->ultNodo == nodoLista)
//                cache->ultNodo = nodoLista->ant;
//
//            /* 2. Conectamos a los vecinos entre si para cerrar el hueco*/
//            if(nodoLista->ant)
//                nodoLista->ant->sig = nodoLista->sig;
//            if(nodoLista->sig)
//                nodoLista->sig->ant = nodoLista->ant;
//
//            /* 3. Lo llevamos al principio */
//            nodoLista->sig = cache->listaDoble;
//            nodoLista->ant = NULL;
//
//            if(cache->listaDoble)
//                cache->listaDoble->ant = nodoLista;
//
//            cache->listaDoble = nodoLista;
//        }
//        return 1;
//    }
//
//    /* 2. Si la cache esta llena, quito el ultimo */
//    if(cache->ce == cache->cap)
//    {
//        tNodo2 *ultimo = cache->ultNodo;
//
//        if(ultimo)
//        {
//            tElemCacheListaDoble *elemUlt = (tElemCacheListaDoble *)ultimo->dato;
//            tIndice indBorrar;
//
//            indBorrar.clave = elemUlt->clave;
//            indBorrar.nodoLista = NULL;
//            indBorrar.cmp = cache->cmp;
//
//            /* Borramos el indice del arbol */
//            eliminarNodo(&cache->arbol, &indBorrar, sizeof(tIndice), cmpIndices);
//
//            /* Actualizamos la cola de la lista */
//            cache->ultNodo = ultimo->ant;
//
//            if(ultimo->ant) /* conecto el ultimo*/
//                ultimo->ant->sig = NULL;
//            else /*si ant es null, significa que la lista esta vacia*/
//                cache->listaDoble = NULL;
//
//            free(elemUlt->clave);
//            free(elemUlt->dato);
//            free(ultimo->dato);
//            free(ultimo);
//
//            cache->ce--;
//        }
//    }
//
//    /* 3. Insertamos el nuevo dato al inicio */
//
//    indNuevo.nodoLista = (tNodo2 *)malloc(sizeof(tNodo2));
//    if (!indNuevo.nodoLista)
//        return 0;
//
//    elemNuevo = (tElemCacheListaDoble *)malloc(sizeof(tElemCacheListaDoble));
//    if (!elemNuevo)
//    {
//        free(indNuevo.nodoLista);
//        return 0;
//    }
//
//    elemNuevo->clave = malloc(cache->tamClave);
//    if (!elemNuevo->clave)
//    {
//        free(indNuevo.nodoLista);
//        free(elemNuevo);
//        return 0;
//    }
//
//    elemNuevo->dato = malloc(cache->tamDato);
//    if (!elemNuevo->dato)
//    {
//        free(indNuevo.nodoLista);
//        free(elemNuevo->clave);
//        free(elemNuevo);
//        return 0;
//    }
//
//    memcpy(elemNuevo->clave, clave, cache->tamClave);
//    memcpy(elemNuevo->dato, dato, cache->tamDato);
//
//    indNuevo.nodoLista->dato = elemNuevo;
//    indNuevo.nodoLista->tam = sizeof(tElemCacheListaDoble);
//    /* el nodo nuevo tiene que ser el primero porque es el ultimo que entro*/
//    indNuevo.nodoLista->ant = NULL;
//    indNuevo.nodoLista->sig = cache->listaDoble;
//
//    if (cache->listaDoble)
//        cache->listaDoble->ant = indNuevo.nodoLista;
//    else
//        cache->ultNodo = indNuevo.nodoLista;
//
//    cache->listaDoble = indNuevo.nodoLista;
//
//    /* Guardo el indice en el arbol */
//    indNuevo.clave = elemNuevo->clave;
//    indNuevo.cmp = cache->cmp;
//
//    insertarOrdenado(&cache->arbol, &indNuevo, sizeof(tIndice), cmpIndices);
//
//    cache->ce++;
//
//    return 1;
//}
////Agregar dividida en 3
void actualizarNodo(tCacheListaDoble *cache, tNodo2 *nodoLista, const void *dato)
{
    tElemCacheListaDoble *elem = (tElemCacheListaDoble *)nodoLista->dato;
    memcpy(elem->dato, dato, cache->tamDato);

    if(nodoLista != cache->listaDoble)
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
}

void sacarUltNodo(tCacheListaDoble *cache)
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

int insertarNuevo(tCacheListaDoble *cache, const void *clave, const void *dato)
{
    tIndice indNuevo;
    tElemCacheListaDoble *elemNuevo;
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
void moverAlPrincipio(tCacheListaDoble *cache, tNodo2 *nodoLista)
{
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
}
int agregarLruCacheListaDobleyArbol(tCacheListaDoble *cache, const void *clave, const void *dato)
{
    //buscarNodoEnArbol
    tArbol *nodoArbol = buscarNodoEnArbol(cache, clave);
    //me devuelve el nodo en el arbol
    //buscarNodoEnArbol
    //veo si el nodo ya existe
    if(nodoArbol && *nodoArbol)
    {
        tIndice *indEncontrado = (tIndice *)(*nodoArbol)->dato;
        tNodo2 *nodoLista = indEncontrado->nodoLista;
        tElemCacheListaDoble *elem = (tElemCacheListaDoble *)nodoLista->dato;

        /* actualizo el dato con el que me llega por parametro */
        memcpy(elem->dato, dato, cache->tamDato);

        /* la consigna me dice que lo tengo que traer al principio */
        moverAlPrincipio(cache, nodoLista);

        return 1;
    }

    /* 2. Si la cache esta llena, quito el ultimo */
    if(cache->ce == cache->cap)
    {
        sacarUltNodo(cache);
    }

    /* 3. Insertamos el nuevo dato al inicio */

    return insertarNuevo(cache, clave, dato);
}
//Agregar dividida en 3

//
//int obtenerLruCacheListaDobleyArbol(tCacheListaDoble *cache, const void *clave, void *dato)
//{
//    tIndice indBusq;
//    tArbol *nodoArbol;
//
//    if(!cache)
//        return 0;
//
//    indBusq.clave = (void *)clave; /* Ya tengo la clave del nodo*/
//    indBusq.nodoLista = NULL;
//    indBusq.cmp = cache->cmp;
//
//    nodoArbol = buscarClave(&cache->arbol, cmpIndices, &indBusq);
//
//    if(nodoArbol && *nodoArbol)
//    {
//        tIndice *indEncontrado = (tIndice *)(*nodoArbol)->dato;
//        tNodo2 *nodoLista = indEncontrado->nodoLista;
//        tElemCacheListaDoble *elem;
//
//        /* Si no esta en la cabeza, lo muevo al principio */
//        if(cache->listaDoble != nodoLista)
//        {
//            if(cache->ultNodo == nodoLista)
//                cache->ultNodo = nodoLista->ant;
//
//            if(nodoLista->ant) /* Si existe un nodo antes tnego que enlazarlo con el siguiente, porque saco al del medio*/
//                nodoLista->ant->sig = nodoLista->sig;
//            if(nodoLista->sig) /* lo mismo que antes pero al reves*/
//                nodoLista->sig->ant = nodoLista->ant;
//
//            nodoLista->sig = cache->listaDoble; /* hago que el que se obtiene ahora apunte al inicio de la cache*/
//            nodoLista->ant = NULL;
//
//            if(cache->listaDoble) /* no esta vacia */
//                cache->listaDoble->ant = nodoLista;/* muevo a segunda posicion*/
//
//            cache->listaDoble = nodoLista; /* ahora es el inicio de la lista*/
//        }
//
//        elem = (tElemCacheListaDoble *)nodoLista->dato;
//        memcpy(dato, elem->dato, cache->tamDato);
//        return 1;
//    }
//
//    return 0;
//}
//la función de obtener más chica
int obtenerLruCacheListaDobleyArbol(tCacheListaDoble *cache, const void *clave, void *dato)
{
    tArbol *nodoArbol = buscarNodoEnArbol(cache, clave);

    if(nodoArbol && *nodoArbol)
    {
        tIndice *indEncontrado = (tIndice *)(*nodoArbol)->dato;
        tNodo2 *nodoLista = indEncontrado->nodoLista;
        tElemCacheListaDoble *elem = (tElemCacheListaDoble *)nodoLista->dato;

        memcpy(dato, elem->dato, cache->tamDato);

        moverAlPrincipio(cache, nodoLista);

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
    vaciarListaDoble(&cache->listaDoble);//aca se vacian los elem
    vaciarArbol(&cache->arbol);

    cache->ultNodo = NULL;
    cache->ce = 0;

    return 1;
}
///BORRAR CACHE Y LISTA DOBLE

//int borrarLruCacheListaDobleyArbol(tCacheListaDoble *cache, const void *clave)
//{
//    tIndice indBorrar;
//    tArbol *pSubArbol;
//    tIndice *indEncontrado;
//    tNodo2 *nodoLista;
//    tElemCacheListaDoble *elem;
//
//    if(!cache)
//        return 0;
//
//    indBorrar.clave = (void *)clave;
//    indBorrar.nodoLista = NULL;
//    indBorrar.cmp = cache->cmp;
//
//    pSubArbol = buscarClave(&cache->arbol, cmpIndices, &indBorrar);
//    if(!pSubArbol || !*pSubArbol) /* si el arbol no tiene elementos o no se encontro el elemento*/
//        return 0; /* No encontre la clave */
//
//    indEncontrado = (tIndice *)(*pSubArbol)->dato;
//    nodoLista = indEncontrado->nodoLista; /* obtenemos el puntero al nodo de la lista guardado en el arbol*/
//
//    /* 1. Desenlazo el nodo de la lista doble */
//    if(nodoLista->ant)
//        nodoLista->ant->sig = nodoLista->sig;
//    else
//        cache->listaDoble = nodoLista->sig;
//
//    if(nodoLista->sig)
//        nodoLista->sig->ant = nodoLista->ant;
//
//    if(cache->ultNodo == nodoLista)
//        cache->ultNodo = nodoLista->ant;
//
//    /* 2. Elimino del arbol */
//    eliminarNodo(&cache->arbol, &indBorrar, sizeof(tIndice), cmpIndices);
//
//    /* 3. Elimino el nodo de la lista y sus datos */
//    elem = (tElemCacheListaDoble *)nodoLista->dato;
//    if(elem)
//    {
//        free(elem->clave);
//        free(elem->dato);
//        free(elem);
//    }
//    free(nodoLista);
//
//    cache->ce--;
//    return 1;
//}
void conectarNodo(tCacheListaDoble *cache, tNodo2 *nodoLista)
{
    if(nodoLista->ant)
        nodoLista->ant->sig = nodoLista->sig;
    else
        cache->listaDoble = nodoLista->sig;

    if(nodoLista->sig)
        nodoLista->sig->ant = nodoLista->ant;

    if(cache->ultNodo == nodoLista)
        cache->ultNodo = nodoLista->ant;
}

void borrarNodoLista(tNodo2 *nodoLista)
{
    tElemCacheListaDoble *elem = (tElemCacheListaDoble *)nodoLista->dato;
    if(elem)
    {
        free(elem->clave);
        free(elem->dato);
        free(elem);
    }
    free(nodoLista);
}

int borrarLruCacheListaDobleyArbol(tCacheListaDoble *cache, const void *clave)
{
    tIndice *indEncontrado;
    tNodo2 *nodoLista;
    tIndice indBorrar;

    tArbol *pSubArbol = buscarNodoEnArbol(cache, clave);

    if(!pSubArbol || !*pSubArbol) /* si el arbol no tiene elementos o no se encontro el elemento*/
        return 0; /* No encontre la clave */

    indEncontrado = (tIndice *)(*pSubArbol)->dato;
    nodoLista = indEncontrado->nodoLista; /* obtenemos el puntero al nodo de la lista guardado en el arbol*/

    /* 1. Desenlazo el nodo de la lista doble */
    conectarNodo(cache, nodoLista);

    /* 2. Elimino del arbol */
    indBorrar.clave = (void *)clave;
    indBorrar.cmp = cache->cmp;
    eliminarNodo(&cache->arbol, &indBorrar, sizeof(tIndice), cmpIndices);

    /* 3. Elimino el nodo de la lista y sus datos */
    borrarNodoLista(nodoLista);
    cache->ce--;
    return 1;
}

int alturaIndiceLruCacheListaDobleyArbol(const tCacheListaDoble *cache)
{
    if(!cache)
        return 0;

    return alturaArbol(&cache->arbol);
}


///Funciones para defensa
/*
1. La función "Peek" (Lectura silenciosa)Contexto: En sistemas reales, a veces el administrador necesita consultar un dato de la caché para auditoría,
pero sin alterar el orden de prioridad temporal (es decir, sin convertirlo en el Más Recientemente Usado).
Consigna: Escribir la función int consultarPeekLruCache(tCacheListaDoble *cache, const void *clave, void *dato).
Qué evalúan: Tu capacidad para reutilizar la lógica de búsqueda en el árbol de índices, extraer el dato copiándolo en la variable de destino,
pero omitiendo deliberadamente la llamada a moverAlPrincipio.
*/

int consultarPeekLruCache(tCacheListaDoble *cache, const void *clave, void *dato)//lo mismo q obtener pero sin poner al principio
{
    tArbol *nodoArbol = buscarNodoEnArbol(cache, clave);
    if(nodoArbol && *nodoArbol)
    {
        tIndice *indEncontrado = (tIndice *)(*nodoArbol)->dato;
        tNodo2 *nodoLista = indEncontrado->nodoLista;
        tElemCacheListaDoble *elem = (tElemCacheListaDoble *)nodoLista->dato;

        memcpy(dato, elem->dato, cache->tamDato);

        return 1;
    }
    return 0;
}

/*2. Redimensionamiento en caliente (El desafío complejo)Contexto: El servidor de "Tweener" recibió una actualización de hardware y ahora tiene más RAM, o por el contrario,
entró en modo ahorro de energía y necesita liberar memoria.Consigna: Escribir la función int redimensionarLruCache(tCacheListaDoble *cache, size_t nuevaCapacidad).
Qué evalúan: Si nuevaCapacidad es mayor, solo se actualiza cache->cap. El verdadero desafío es si la nueva capacidad es menor a la cantidad de elementos actuales (cache->ce).
En ese caso, debés programar un ciclo while que llame repetidamente a tu función de desalojo (sacarUltNodo) hasta que ce == nuevaCapacidad.*/

int redimensionarLruCache(tCacheListaDoble *cache, size_t nuevaCapacidad)
{
    if(!cache->listaDoble || nuevaCapacidad == 0)//si no tengo lista o la cap es 0 no puedo hacer nada
        return 0;

    cache->cap = nuevaCapacidad;//siempre tengo q actualizar la cap

    while(cache->ce > nuevaCapacidad)
    {
        sacarUltNodo(cache);//saco el ultimo hasta que la capacidad sea igual a los ce
    }

    return 1;
}

/*
3. Recorrido de auditoría (MRU a LRU)Contexto: Para verificar que la lista doble está manejando correctamente el orden temporal,
se te pide imprimir las claves en pantalla desde la más nueva hasta la más vieja a punto de ser eliminada.Consigna: Suponiendo que las claves son números enteros,
escribí una función void imprimirEstadoCache(const tCacheListaDoble *cache) que recorra la lista y haga un printf de cada clave.Qué evalúan: El manejo básico de listas enlazadas.
Quieren ver si escribís correctamente el ciclo while(actual != NULL), cómo casteás el puntero actual->dato hacia tElemCacheListaDoble*, y cómo extraés la clave casteándola a int* antes de imprimirla.
*/
void imprimirEstadoCache(const tCacheListaDoble *cache)
{
    tNodo2 *nodoLista = cache->listaDoble;

    while(nodoLista != NULL)
    {
        tElemCacheListaDoble *elem = (tElemCacheListaDoble *)nodoLista->dato;
        int clave = *(int *)elem->clave;
        printf("%d \n", clave);
        nodoLista = nodoLista->sig;
    }
}

/*
4. Análisis del Árbol de ÍndicesContexto: Como el benchmark demostró que el árbol se puede degenerar, el profesor quiere saber cuántos nodos en tu árbol quedaron como "hojas"
(nodos sin hijos izquierdo ni derecho), ya que un árbol degenerado tiene una sola hoja, mientras que uno balanceado tiene muchas.
Consigna: Diseñar la función int contarHojasArbolIndices(const tArbol *arbol) para el TDA Árbol.
Qué evalúan: Recursividad estricta en C. Tenés que identificar el caso base (si el árbol está vacío, retorna 0),
el caso hoja (si no tiene hijo izquierdo ni derecho, retorna 1), y el paso recursivo (retornar la suma de contar las hojas de la rama izquierda más las de la rama derecha)
*/
int contarHojasArbolIndices(const tArbol *arbol)
{
    if(!*arbol)
         return 0;
    if(!(*arbol)->izq && !(*arbol)->der)
        return 1;

    return contarHojasArbolIndices(&(*arbol)->izq) + contarHojasArbolIndices(&(*arbol)->der);
 }

 /*
 5. Obtener los "N" más recientes (Ideal para el Feed de "Tweener")
En una red social no sacás un solo posteo, a veces querés cargar los últimos 5 o 10 más recientes que están en memoria.
Esta función recorre la lista desde la cabecera (los más nuevos) y copia los datos en un vector.
 */
int obtenerTopNRecientes(const tCacheListaDoble *cache, int n, void *vectorSalida)
{
    if(!cache || !vectorSalida)
        return 0;
    tNodo2 *actual = cache->listaDoble;
    int cant = 0;
    char *v = (char *)vectorSalida;

    while(actual != NULL && cant < n)
    {
        tElemCacheListaDoble *elem = (tElemCacheListaDoble *)actual->dato;
        memcpy(v + (cant*cache->tamDato), elem->dato, cache->tamDato);

        actual = actual->sig;
        cant++;
    }

    return cant;
}


//lo mismo pero con una lista simple
int obtenerTopNRecientesLista(const tCacheListaDoble *cache, int n, tNodo1 **lista)
{
    if(!cache || !lista)
        return 0;

    tNodo2 *actual = cache->listaDoble;
    int cant = 0;

    tNodo1 **ult = lista;

    while(actual != NULL && cant < n)
    {
        tElemCacheListaDoble *elem = (tElemCacheListaDoble *)actual->dato;

        tNodo1 *nuevo = (tNodo1 *)malloc(sizeof(tNodo1));
        if(!nuevo)
            return cant;

        nuevo->info = malloc(cache->tamDato);
        if(!nuevo->info)
        {
            free(nuevo);
            return cant;
        }

        memcpy(nuevo->info, elem->dato, cache->tamDato);
        nuevo->tamInfo = cache->tamDato;
        nuevo->sig = NULL;

        *ult = nuevo;

        ult = &nuevo->sig;

        actual = actual->sig;
        cant++;
    }

    return cant;
}

int obtenerTopNUltimosLista(const tCacheListaDoble *cache, int n, tNodo1 **lista)
{
    if(!cache || !lista)
        return 0;
    tNodo2 *actual = cache->ultNodo;
    int cant = 0;

    tNodo1 **pri = lista;

    while(actual != NULL && cant < n)
    {
        tElemCacheListaDoble *elem = (tElemCacheListaDoble *)actual->dato;

        tNodo1 *nuevo = (tNodo1 *)malloc(sizeof(tNodo1));
        if(!nuevo)
            return cant;

        nuevo->info = malloc(cache->tamDato);
        if(!nuevo->info)
        {
            free(nuevo);
            return cant;
        }

        memcpy(nuevo->info, elem->dato, cache->tamDato);
        nuevo->tamInfo = cache->tamDato;
        nuevo->sig = NULL;

        *pri = nuevo;

        pri = &nuevo->sig;

        actual = actual->ant;
        cant++;
    }

    return cant;
}
/*
2. Sanity Check: Validación de Consistencia
Como tenés dos estructuras corriendo en paralelo (la Lista y el Árbol), un bug crítico sería que un
elemento se borre de la lista pero quede "fantasma" en el árbol.
Esta función verifica que el árbol y la lista tengan exactamente la misma cantidad de elementos.
*/
int contarNodosArbol(const tArbol *arbol)
{
    if(!*arbol)
        return 0;

    return 1 + contarNodosArbol(&(*arbol)->izq) + contarNodosArbol(&(*arbol)->der);//sumo subarbol izq con subarbol der
}

int validarConsistenciaCache(const tCacheListaDoble *cache)
{
    int nodosEnArbol = contarNodosArbol(&cache->arbol);

    //m fijo si los ce son iguales a los ndoos en el arbol
    return (cache->ce == nodosEnArbol) ? 1 : 0;
}

/*
Invertir la Caché (La trampa del pizarrón)
Esta no tiene utilidad práctica en el negocio de una caché (destruye el orden temporal LRU),
pero es la pregunta de examen por excelencia para probar si dominás las listas doblemente enlazadas.
Intercambia los punteros sig y ant de cada nodo.
*/

void invertirOrdenCache(tCacheListaDoble *cache)
{
    tNodo2 *actual = cache->listaDoble;
    tNodo2 *prev = NULL;
    tNodo2 *next = NULL;
    cache->ultNodo = cache->listaDoble;

    while(actual)
    {
        next = actual->sig;

        actual->ant = next;
        actual->sig = prev;

        prev = actual;
        actual = next;

    }

    cache->listaDoble = prev;
}
