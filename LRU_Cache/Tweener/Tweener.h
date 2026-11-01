#ifndef TWEENER_H_INCLUDED
#define TWEENER_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../TDA_Cache_Lista_Simple/TDA_Cache_Lista.h"
#include "../TDA_CACHE_OPTIMIZADA/TDA_CACHE_OPTIMIZADA.h"

#define TAM_TEXTO_POSTEO 141
#define TOTAL_POSTEOS 50000

#define TAM_NOMBRE_USUARIO 16
#define CANT_FEED_USUARIO 20
#define MIN_USUARIOS 200

#define LOTE_CONCENTRADO 1
#define LOTE_DISPERSO    2
#define LOTE_ORDENADO    3

typedef struct {
    int id;
    char texto[TAM_TEXTO_POSTEO];
} tPosteo;

typedef struct {
    char usuario[TAM_NOMBRE_USUARIO];
    int feed[CANT_FEED_USUARIO];
} tUsuario;

/* Estructura para las metricas */
typedef struct {
    int hits;
    int misses;
    int lecturasDisco;
    double tiempoTotal;
    double tiempoPorUsuario;
    double tiempoPorPosteo;
    int alturaArbol;
} tMetricasSimulacion;

/* Funcion de comparacion de claves para las caches */
int compararIds(const void *a, const void *b);

/* Generadores de archivos de prueba */
int generarArchivoPosteos(const char *nombreArchivo, int cantidad);
int generarLoteUsuarios(const char *nombreArchivo, int cantUsuarios, int tipoLote, unsigned int semilla);

/* Simulacion de Tweener */
int simularSinCache(const char *nomArchPosteos, const char *nomArchUsuarios, tMetricasSimulacion *metricas);
int simularCacheParte1(const char *nomArchPosteos, const char *nomArchUsuarios, size_t capCache, tMetricasSimulacion *metricas);
int simularCacheParte2(const char *nomArchPosteos, const char *nomArchUsuarios, size_t capCache, tMetricasSimulacion *metricas);

/* imprimir reporte de metricas */
void imprimirReporte(const char *titulo, const tMetricasSimulacion *metricas, int totalUsuarios);

#endif /* TWEENER_H_INCLUDED */
