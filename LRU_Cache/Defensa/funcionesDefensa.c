#include "funcionesDefensa.h"

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

