#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Tweener/Tweener.h"
#include "Defensa/funcionesDefensa.h"
//static void probarLote(const char *nomArchPosteos, const char *nomArchUsuarios,
//                       const char *descLote, int cantUsuarios,
//                       const size_t *capacidades, int cantCapacidades)
//{
//    tMetricasSimulacion metricas;
//    const size_t *pCap;
//    const size_t *pFinCap;
//    char titulo[128];
//
//    printf("\n======================================================================\n");
//    printf("EVALUANDO: %s\n", descLote);
//    printf("Usuarios: %d | Total de consultas: %d\n", cantUsuarios, cantUsuarios * CANT_FEED_USUARIO);
//    printf("======================================================================\n\n");
//
//    /* 1. Medicion SIN CACHE */
//    if (simularSinCache(nomArchPosteos, nomArchUsuarios, &metricas))
//    {
//        imprimirReporte("MODO: Sin Cache (Lectura directa de disco)", &metricas, cantUsuarios);
//    }
//
//    pFinCap = capacidades + cantCapacidades;
//
//    /* 2. Medicion con CACHE PARTE 1 (Lista Simple) para cada capacidad */
//    pCap = capacidades;
//    while (pCap < pFinCap)
//    {
//        sprintf(titulo, "MODO: Cache Parte 1 (Lista Simple) - Capacidad: %lu", (unsigned long)*pCap);
//        if (simularCacheParte1(nomArchPosteos, nomArchUsuarios, *pCap, &metricas))
//        {
//            imprimirReporte(titulo, &metricas, cantUsuarios);
//        }
//        pCap++;
//    }
//
//    /* 3. Medicion con CACHE PARTE 2 (Lista Doble + Arbol) para cada capacidad */
//    pCap = capacidades;
//    while (pCap < pFinCap)
//    {
//        sprintf(titulo, "MODO: Cache Parte 2 (Lista Doble + Arbol) - Capacidad: %lu", (unsigned long)*pCap);
//        if (simularCacheParte2(nomArchPosteos, nomArchUsuarios, *pCap, &metricas))
//        {
//            imprimirReporte(titulo, &metricas, cantUsuarios);
//        }
//        pCap++;
//    }
//}
//
//int main()
//{
//    const char *nomPosteos = "posteos.bin";
//    const char *nomConc = "usuarios_concentrado.bin";
//    const char *nomDisp = "usuarios_disperso.bin";
//    const char *nomOrd  = "usuarios_ordenado.bin";
//
//    int cantUsuarios = 50000;
//    unsigned int semilla = 2026;
//    size_t capacidades[] = { 50, 200, 1000 };
//    int cantCapacidades = 3;
//    FILE *pfTest;
//
//    printf("======================================================================\n");
//    printf("       SIMULACION RED SOCIAL 'TWEENER' - BENCHMARK LRU CACHE         \n");
//    printf("======================================================================\n\n");
//
//    /* 1. Verificamos si existe el archivo de posteos; si no existe, lo creamos */
//    pfTest = fopen(nomPosteos, "rb");
//    if (!pfTest)
//    {
//        printf("Creando '%s' con %d posteos...\n", nomPosteos, TOTAL_POSTEOS);
//        if (!generarArchivoPosteos(nomPosteos, TOTAL_POSTEOS))
//        {
//            printf("Error critico al generar archivo de posteos.\n");
//            return 1;
//        }
//    }
//    else
//    {
//        fclose(pfTest);
//        printf("Archivo de posteos '%s' verificado.\n", nomPosteos);
//    }
//
//    /* 2. Generamos los 3 lotes de prueba con la semilla fija requerida */
//    printf("Generando lotes de prueba con semilla %u...\n", semilla);
//    generarLoteUsuarios(nomConc, cantUsuarios, LOTE_CONCENTRADO, semilla);
//    generarLoteUsuarios(nomDisp, cantUsuarios, LOTE_DISPERSO, semilla);
//    generarLoteUsuarios(nomOrd,  cantUsuarios, LOTE_ORDENADO, semilla);
//    printf("Lotes generados con exito.\n");
//
//    /* 3. Ejecucion de simulaciones por cada lote */
//    probarLote(nomPosteos, nomConc, "LOTE CONCENTRADO (Alta tasa de Hits)", cantUsuarios, capacidades, cantCapacidades);
//    probarLote(nomPosteos, nomDisp, "LOTE DISPERSO (Baja tasa de Hits)", cantUsuarios, capacidades, cantCapacidades);
//    probarLote(nomPosteos, nomOrd,  "LOTE ORDENADO (Prueba de degeneracion del ABB)", cantUsuarios, capacidades, cantCapacidades);
//
//    printf("\nSimulacion completada exitosamente.\n");
//    return 0;
//}

/* Estructura de negocio (lo que guarda el usuario) */
typedef struct {
    int idPosteo;
    char texto[50];
} tPosteo2;

/* Función de comparación para el árbol y la caché */
int cmpClavesInt(const void *a, const void *b) {
    return *(int*)a - *(int*)b;
}

/* Criterio para mapCache: promueve los posteos con ID par */
int esPosteoPar(const void *dato) {
    tPosteo2 *p = (tPosteo2 *)dato;
    return p->idPosteo % 2 == 0;
}

/* Helper para visualizar la Lista Doble de la caché */
void imprimirCache(tCacheListaDoble *cache) {
    tNodo2 *act = cache->listaDoble;
    printf("MRU -> ");
    while(act) {
        tElemCacheListaDoble *elem = (tElemCacheListaDoble *)act->dato;
        tPosteo2 *p = (tPosteo2 *)elem->dato;
        int clave = *(int*)elem->clave;
        printf("[%d: %s] ", clave, p->texto);
        act = act->sig;
    }
    printf("-> LRU\n");
}

/* Helper para visualizar la lista de salida extraída */
void imprimirListaSalida(tListaDoble *lista) {
    tNodo2 *act = *lista;
    printf("EXTRAIDOS -> ");
    while(act) {
        tElemCacheListaDoble *elem = (tElemCacheListaDoble *)act->dato;
        tPosteo2 *p = (tPosteo2 *)elem->dato;

        printf("[%d: %s] ", *(int*)elem->clave, p->texto);

        act = act->sig;
    }
    printf("\n");
}

int main() {
    /* 1. Inicialización usando tu función real */
    tCacheListaDoble cache;
    crearLruCacheListaDobleyArbol(&cache, 10, sizeof(int), sizeof(tPosteo2), cmpClavesInt);

    tPosteo2 posteos[] = {
        {10, "Post A"}, {15, "Post B"}, {20, "Post C"},
        {25, "Post D"}, {30, "Post E"}
    };

    /* 2. Poblamos la caché con agregarLruCacheListaDobleyArbol */
    for(int i = 0; i < 5; i++) {
        agregarLruCacheListaDobleyArbol(&cache, &posteos[i].idPosteo, &posteos[i]);
    }

    printf("--- ESTADO INICIAL ---\n");
    // Orden esperado si insertaste secuencialmente y siempre van al MRU:
    // [30] [25] [20] [15] [10]
    imprimirCache(&cache);

    /* 3. Test de mapCache (Promoción manteniendo orden relativo) */
    printf("\n--- TEST: mapCache (Promover pares) ---\n");
    mapCache(&cache, esPosteoPar);
    // Orden esperado: [30] [20] [10] [25] [15]
    imprimirCache(&cache);

    /* 4. Test de extraerRangoClaves */
    printf("\n--- TEST: extraerRangoClaves (Min: 15, Max: 25) ---\n");
    tListaDoble listaExtraidos = NULL;
    int min = 15, max = 25;

    int cant = extraerRangoClaves(&cache, &min, &max, &listaExtraidos);
    printf("Nodos extraidos: %d\n", cant);

    // Caché remanente esperada: [30] [10] (el 20, 25 y 15 fueron arrancados)
    printf("Caché despues de extraer:\n");
    imprimirCache(&cache);

    // Lista de salida esperada: [20] [25] [15]
    imprimirListaSalida(&listaExtraidos);

    /* 5. Destrucción final */
    vaciarLruCacheListaDobleyArbol(&cache);

    // Vaciado manual simple para la lista de extraídos (como ya no están en la caché)
    tNodo2 *act = listaExtraidos;
    while(act) {
        tNodo2 *sig = act->sig;
        borrarNodoLista(act); // Reutilizamos tu helper
        act = sig;
    }

    return 0;
}
