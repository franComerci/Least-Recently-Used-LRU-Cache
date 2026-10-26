#include "comun.h"
#include "Utilitiarias.h"

/// FUNCIÓN: cmpInt
/// DESCRIPCION Función de comparación para enteros.
int cmpInt(const void *a, const void *b)
{
    int *claveA = (int*)a;
    int *claveB = (int*)b;
    return *claveA - *claveB;
}

///FUNCION mostrarCache
///DESCRIPCION Muestra el estado de la cache
void mostrarCache(tCache *cache)
{
    tNodo *actual = cache->lista;
    printf("Estado Cache (%lu/%lu):\n", (unsigned long)cache->ce, (unsigned long)cache->cap);

    if (actual == NULL) {
        printf("  (Cache vacia)\n");
    }

    while (actual != NULL)
    {
        tElemCache *elem = (tElemCache*)actual->info;
        int *clave = (int*)elem->clave;
        char *dato = (char*)elem->dato;

        printf(" -> [Clave: %d | Dato: %s]\n", *clave, dato);
        actual = actual->sig;
    }
    printf("------------------------\n");
}
