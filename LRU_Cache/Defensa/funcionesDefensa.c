#include "funcionesDefensa.h"
///Funciones para defensa
/*
La función "Peek" (Lectura silenciosa)Contexto: En sistemas reales, a veces el administrador necesita consultar un dato de la caché para auditoría,
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

/*
Redimensionamiento en caliente (El desafío complejo)Contexto: El servidor de "Tweener" recibió una actualización de hardware y ahora tiene más RAM, o por el contrario,
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
Recorrido de auditoría (MRU a LRU)Contexto: Para verificar que la lista doble está manejando correctamente el orden temporal,
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
Análisis del Árbol de ÍndicesContexto: Como el benchmark demostró que el árbol se puede degenerar, el profesor quiere saber cuántos nodos en tu árbol quedaron como "hojas"
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
Obtener los "N" más recientes (Ideal para el Feed de "Tweener")
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
Sanity Check: Validación de Consistencia
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

/*

Recorre la caché y elimina de la lista doble y del árbol todos los posteos o elementos cuyo dato cumpla con la condición inyectada por la función filtro
(ejemplo: posteos de un usuario baneado en la red social). Retorna la cantidad de elementos eliminados.
Trampa: Al borrar elementos en el medio de la iteración, tenés que resguardar el puntero actual->sig antes de hacer el free para no perder el hilo de la lista.
*/
int filtroCache(const void *dato)
{
    tPosteo *elem = (tPosteo *)dato;
    return elem->id % 2;
}
int cacheFiltro(tCacheListaDoble *cache, int (*filtro)(const void *dato))
{
    int ce = 0;
    if(!cache)
        return -1;
    tNodo2 *nodoLista = cache->listaDoble;

    while(nodoLista)
    {
        tElemCacheListaDoble *elem = (tElemCacheListaDoble *)nodoLista->dato;

        tNodo2 *sig = nodoLista->sig; //tengo que guardarlo acá porque llamo a borrar lruCache y se va el nodo
        if(filtro(elem->dato))
        {
            borrarLruCacheListaDobleyArbol(cache, elem->clave);
            ce++;
        }

        nodoLista = sig;
    }

    return ce;
}

/*
Promoción en Bloque (Bulk MRU)int promoverBloqueMRU(tCacheListaDoble *cache, int (*criterio)(const void *dato));
Mueve al principio de la lista doble (posición MRU) a todos los elementos que cumplan el criterio, pero manteniendo el orden relativo que tenían entre ellos.
No se elimina nada, solo se desenlazan y reenlazan los nodos.
*/

int mapCache(tCacheListaDoble *cache, int (*criterio)(const void *dato))
{
    if(!cache || cache->ce == 0)
        return 0;

    //vector para guardar los elementos
    tNodo2 **aMover = malloc(cache->ce * sizeof(tNodo2 *));
    if(!aMover)
        return 0;

    int cant = 0;
    tNodo2 *nodoLista = cache->ultNodo;

    //recorro de atras para adelante para quedarme con el orden
    while(nodoLista)
    {
        tElemCacheListaDoble *elem = (tElemCacheListaDoble *)nodoLista->dato;
        tNodo2 *ant = nodoLista->ant;

        if(criterio(elem->dato))
        {
            *(aMover + cant) = nodoLista;
            cant++;
        }
        nodoLista = ant;
    }

    //los cargo en la lista
    for(int i = 0; i < cant; i++)
    {
        moverAlPrincipio(cache, *(aMover + i));
    }

    free(aMover);

    return 1;
}
/*
Extracción de Rango de Claves int extraerRangoClaves(tCacheListaDoble *cache, void *claveMin, void *claveMax, tListaDoble *listaSalida);
Utilizando la eficiencia del árbol binario de índices, encontrá todos los elementos cuyas claves estén entre claveMin y claveMax (inclusives).
Desenlazá esos nodos de la caché (borrándolos del árbol y de la lista interna) y enganchalos ordenados en la listaSalida.
*/

int extraerRangoClaves(tCacheListaDoble *cache, void *claveMin, void *claveMax, tListaDoble *listaSalida)
{
    if(!cache || !listaSalida) return 0;

    int cant = 0;
    tNodo2 *actual = cache->listaDoble;

    /* Posicionamiento en el último nodo de la lista de salida */
    tNodo2 *ultSalida = *listaSalida;
    while(ultSalida && ultSalida->sig) ultSalida = ultSalida->sig;

    while(actual)
    {
        tNodo2 *sig = actual->sig;
        tElemCacheListaDoble *elem = (tElemCacheListaDoble *)actual->dato;

        if(cache->cmp(elem->clave, claveMin) >= 0 && cache->cmp(elem->clave, claveMax) <= 0)
        {
            /* Desenlazar de la caché */
            conectarNodo(cache, actual);
            eliminarNodo(&cache->arbol, elem->clave, cache->tamDato, cache->cmp);
            cache->ce--;

            /* Enlazar directamente a listaSalida reciclando el tNodo2 exacto */
            actual->sig = NULL;
            actual->ant = ultSalida;

            if(ultSalida) ultSalida->sig = actual;
            else *listaSalida = actual;

            ultSalida = actual;
            cant++;
        }
        actual = sig;
    }
    return cant;
}

/*
Serialización Segura a Binario
int volcarCacheBinario(const tCacheListaDoble *cache, const char *rutaArchivo);
Escribe el estado actual de la caché en un archivo binario. Debe guardar un encabezado con la cantidad de elementos y los tamaños (tamClave, tamDato), seguido de los bloques de memoria cruda de cada elemento en el orden exacto de MRU a LRU.
Trampa: No podés guardar los nodos (que tienen punteros), tenés que extraer y escribir únicamente los void *clave y void *dato de cada tElemCacheListaDoble.
*/


int volcarCacheBinario(const tCacheListaDoble *cache, const char *rutaArchivo)
{
    FILE *arch = fopen(rutaArchivo, "wb");
    if(!arch)
        return 0;

    fwrite(&cache->ce, sizeof(cache->ce), 1, arch);
    fwrite(&cache->tamClave, sizeof(cache->tamClave), 1, arch);
    fwrite(&cache->tamDato, sizeof(cache->tamDato), 1, arch);
    tNodo2 *nodoLista = cache->listaDoble;

    while(nodoLista)
    {
        tElemCacheListaDoble *elem = (tElemCacheListaDoble *)nodoLista->dato;
        fwrite(elem->clave, cache->tamClave, 1, arch);
        fwrite(elem->dato, cache->tamDato, 1, arch);
        nodoLista = nodoLista->sig;
    }

    fclose(arch);

    return 1;
}

/*
Clonación Profunda (Deep Clone)
int clonarCacheLru(const tCacheListaDoble *original, tCacheListaDoble *clon);
Crea una copia exacta y funcional de la caché original en una nueva variable clon.
La nueva caché debe tener sus propios nodos de árbol, sus propios nodos de lista y sus propias reservas de memoria (malloc)
para cada clave y dato, copiando el contenido con memcpy.
*/
int clonarCacheLru(const tCacheListaDoble *original, tCacheListaDoble *clon)
{
    if (!original || !clon)
        return 0;

    crearLruCacheListaDobleyArbol(clon, original->cap, original->tamClave, original->tamDato, original->cmp);

    //arranco desde el final para no tener que volver a cargar
    tNodo2 *act = original->ultNodo;

    while (act)
    {
        tElemCacheListaDoble *elem = (tElemCacheListaDoble *)act->dato;
        if (!agregarLruCacheListaDobleyArbol(clon, elem->clave, elem->dato))
        {
            //si falla la memoria, vaciamos
            vaciarLruCacheListaDobleyArbol(clon);
            return 0;
        }

        act = act->ant;
    }

    return 1;
}

/*
Inversión Parcial (Mitad LRU)
void invertirMitadViejaCache(tCacheListaDoble *cache);
A diferencia de tu función de invertir toda la caché, esta función debe encontrar el punto medio exacto de la lista doble y solo invertir los punteros de la mitad más vieja
(la que está cerca de ultNodo), dejando la mitad más nueva intacta.
*/

