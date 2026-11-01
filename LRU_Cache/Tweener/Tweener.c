#include "Tweener.h"

/* Funcion auxiliar para generar enteros en un rango amplio [0, maxExcl - 1] */
/* Nota: En Windows/MinGW RAND_MAX suele ser 32767, por lo que rand() % 50000 */
/* no cubriria valores superiores a 32767 sin este ajuste de bits.            */
static int randRango(int maxExcl)
{
    unsigned int r;

    if (maxExcl <= 0)
        return 0;

    r = ((unsigned int)rand() << 15) | (unsigned int)rand();
    return (int)(r % (unsigned int)maxExcl);
}

/* Comparador de IDs enteros para las caches */
int compararIds(const void *a, const void *b)
{
    return (*(const int *)a - *(const int *)b);
}

/* Comparador para qsort */
static int cmpEnteros(const void *a, const void *b)
{
    return (*(const int *)a - *(const int *)b);
}

/* ========================================================================= */
/* GENERADOR DEL ARCHIVO BINARIO DE POSTEOS (50.000 registros)               */
/* ========================================================================= */
int generarArchivoPosteos(const char *nombreArchivo, int cantidad)
{
    FILE *pf;
    int i;
    tPosteo posteo;

    pf = fopen(nombreArchivo, "wb");
    if (!pf)
    {
        printf("Error: No se pudo crear el archivo de posteos '%s'.\n", nombreArchivo);
        return 0;
    }

    printf("Generando archivo de posteos '%s' con %d registros...\n", nombreArchivo, cantidad);

    for (i = 0; i < cantidad; i++)
    {
        posteo.id = i;
        sprintf(posteo.texto, "Tweener post #%d: Mensaje simulado de prueba para la red social Tweener.", i);

        if (fwrite(&posteo, sizeof(tPosteo), 1, pf) != 1)
        {
            printf("Error de escritura al generar el posteo id=%d.\n", i);
            fclose(pf);
            return 0;
        }
    }

    fclose(pf);
    printf("Archivo de posteos generado exitosamente.\n\n");
    return 1;
}

/* ========================================================================= */
/* GENERADOR DE LOTES DE USUARIOS (Concentrado, Disperso y Ordenado)         */
/* ========================================================================= */
int generarLoteUsuarios(const char *nombreArchivo, int cantUsuarios, int tipoLote, unsigned int semilla)
{
    FILE *pf;
    int i;
    int secuenciaId;
    int *pFeed;
    const int *pFinFeed;
    tUsuario user;

    if (cantUsuarios < MIN_USUARIOS)
        cantUsuarios = MIN_USUARIOS;

    pf = fopen(nombreArchivo, "wb");
    if (!pf)
    {
        printf("Error: No se pudo crear el archivo de usuarios '%s'.\n", nombreArchivo);
        return 0;
    }

    srand(semilla);
    secuenciaId = 0;

    for (i = 0; i < cantUsuarios; i++)
    {
        sprintf(user.usuario, "user_%04d", i + 1);

        pFeed = user.feed;
        pFinFeed = user.feed + CANT_FEED_USUARIO;

        while (pFeed < pFinFeed)
        {
            switch (tipoLote)
            {
                case LOTE_CONCENTRADO:
                    /* Principio 80/20: El 80% de los accesos van a los primeros 50 posteos (virales) */
                    if ((rand() % 100) < 80)
                        *pFeed = randRango(50);
                    else
                        *pFeed = randRango(TOTAL_POSTEOS);
                    break;

                case LOTE_DISPERSO:
                    /* Distribucion uniforme en todo el rango [0, TOTAL_POSTEOS - 1] */
                    *pFeed = randRango(TOTAL_POSTEOS);
                    break;

                case LOTE_ORDENADO:
                    /* IDs en orden estrictamente creciente a traves de los feeds */
                    *pFeed = secuenciaId % TOTAL_POSTEOS;
                    secuenciaId += (rand() % 3) + 1;
                    break;

                default:
                    *pFeed = randRango(TOTAL_POSTEOS);
                    break;
            }
            pFeed++;
        }

        /* Si es lote ordenado, aseguramos que el feed individual quede ordenado creciente */
        if (tipoLote == LOTE_ORDENADO)
        {
            qsort(user.feed, CANT_FEED_USUARIO, sizeof(int), cmpEnteros);
        }

        if (fwrite(&user, sizeof(tUsuario), 1, pf) != 1)
        {
            printf("Error de escritura en usuario %s.\n", user.usuario);
            fclose(pf);
            return 0;
        }
    }

    fclose(pf);
    return 1;
}

/* ========================================================================= */
/* SIMULACION: SIN CACHE (Acceso directo a disco con fseek + fread)          */
/* ========================================================================= */
int simularSinCache(const char *nomArchPosteos, const char *nomArchUsuarios, tMetricasSimulacion *metricas)
{
    FILE *pfPosteos;
    FILE *pfUsuarios;
    tUsuario user;
    tPosteo posteo;
    int cantUsuarios;
    int totalConsultas;
    int id;
    const int *pFeed;
    const int *pFinFeed;
    clock_t inicio;
    clock_t fin;

    pfPosteos = fopen(nomArchPosteos, "rb");
    if (!pfPosteos)
    {
        printf("Error al abrir '%s'.\n", nomArchPosteos);
        return 0;
    }

    pfUsuarios = fopen(nomArchUsuarios, "rb");
    if (!pfUsuarios)
    {
        printf("Error al abrir '%s'.\n", nomArchUsuarios);
        fclose(pfPosteos);
        return 0;
    }

    metricas->hits = 0;
    metricas->misses = 0;
    metricas->lecturasDisco = 0;
    metricas->alturaArbol = 0;
    cantUsuarios = 0;

    inicio = clock();

    while (fread(&user, sizeof(tUsuario), 1, pfUsuarios) == 1)
    {
        cantUsuarios++;
        pFeed = user.feed;
        pFinFeed = user.feed + CANT_FEED_USUARIO;

        while (pFeed < pFinFeed)
        {
            id = *pFeed;
            if (id >= 0 && id < TOTAL_POSTEOS)
            {
                /* Busqueda directa por posicion relativa en el archivo */
                fseek(pfPosteos, (long)id * (long)sizeof(tPosteo), SEEK_SET);
                fread(&posteo, sizeof(tPosteo), 1, pfPosteos);
                metricas->lecturasDisco++;
            }
            metricas->misses++;
            pFeed++;
        }
    }

    fin = clock();

    fclose(pfPosteos);
    fclose(pfUsuarios);

    metricas->tiempoTotal = (double)(fin - inicio) / CLOCKS_PER_SEC;
    totalConsultas = cantUsuarios * CANT_FEED_USUARIO;

    metricas->tiempoPorUsuario = (cantUsuarios > 0) ? (metricas->tiempoTotal / cantUsuarios) : 0.0;
    metricas->tiempoPorPosteo = (totalConsultas > 0) ? (metricas->tiempoTotal / totalConsultas) : 0.0;

    return 1;
}

/* ========================================================================= */
/* SIMULACION: CON CACHE PARTE 1 (Lista Simplemente Enlazada)                */
/* ========================================================================= */
int simularCacheParte1(const char *nomArchPosteos, const char *nomArchUsuarios, size_t capCache, tMetricasSimulacion *metricas)
{
    FILE *pfPosteos;
    FILE *pfUsuarios;
    tCache cache;
    tUsuario user;
    tPosteo posteo;
    int cantUsuarios;
    int totalConsultas;
    int id;
    const int *pFeed;
    const int *pFinFeed;
    clock_t inicio;
    clock_t fin;

    pfPosteos = fopen(nomArchPosteos, "rb");
    if (!pfPosteos)
    {
        printf("Error al abrir '%s'.\n", nomArchPosteos);
        return 0;
    }

    pfUsuarios = fopen(nomArchUsuarios, "rb");
    if (!pfUsuarios)
    {
        printf("Error al abrir '%s'.\n", nomArchUsuarios);
        fclose(pfPosteos);
        return 0;
    }

    crearLruCache(&cache, capCache, sizeof(int), sizeof(tPosteo), compararIds);

    metricas->hits = 0;
    metricas->misses = 0;
    metricas->lecturasDisco = 0;
    metricas->alturaArbol = 0;
    cantUsuarios = 0;

    inicio = clock();

    while (fread(&user, sizeof(tUsuario), 1, pfUsuarios) == 1)
    {
        cantUsuarios++;
        pFeed = user.feed;
        pFinFeed = user.feed + CANT_FEED_USUARIO;

        while (pFeed < pFinFeed)
        {
            id = *pFeed;

            /* 1. Intentamos obtener de la cache */
            if (obtenerLruCache(&cache, &id, &posteo))
            {
                metricas->hits++;
            }
            else
            {
                /* 2. Fallo de cache: Leemos de disco e insertamos en cache */
                metricas->misses++;
                if (id >= 0 && id < TOTAL_POSTEOS)
                {
                    fseek(pfPosteos, (long)id * (long)sizeof(tPosteo), SEEK_SET);
                    fread(&posteo, sizeof(tPosteo), 1, pfPosteos);
                    metricas->lecturasDisco++;

                    agregarLruCache(&cache, &id, &posteo);
                }
            }
            pFeed++;
        }
    }

    fin = clock();

    vaciarLruCache(&cache);
    fclose(pfPosteos);
    fclose(pfUsuarios);

    metricas->tiempoTotal = (double)(fin - inicio) / CLOCKS_PER_SEC;
    totalConsultas = cantUsuarios * CANT_FEED_USUARIO;

    metricas->tiempoPorUsuario = (cantUsuarios > 0) ? (metricas->tiempoTotal / cantUsuarios) : 0.0;
    metricas->tiempoPorPosteo = (totalConsultas > 0) ? (metricas->tiempoTotal / totalConsultas) : 0.0;

    return 1;
}

/* ========================================================================= */
/* SIMULACION: CON CACHE PARTE 2 (Lista Doblemente Enlazada + Arbol ABB)     */
/* ========================================================================= */
int simularCacheParte2(const char *nomArchPosteos, const char *nomArchUsuarios, size_t capCache, tMetricasSimulacion *metricas)
{
    FILE *pfPosteos;
    FILE *pfUsuarios;
    tCacheListaDoble cache;
    tUsuario user;
    tPosteo posteo;
    int cantUsuarios;
    int totalConsultas;
    int id;
    const int *pFeed;
    const int *pFinFeed;
    clock_t inicio;
    clock_t fin;

    pfPosteos = fopen(nomArchPosteos, "rb");
    if (!pfPosteos)
    {
        printf("Error al abrir '%s'.\n", nomArchPosteos);
        return 0;
    }

    pfUsuarios = fopen(nomArchUsuarios, "rb");
    if (!pfUsuarios)
    {
        printf("Error al abrir '%s'.\n", nomArchUsuarios);
        fclose(pfPosteos);
        return 0;
    }

    crearLruCacheListaDobleyArbol(&cache, capCache, sizeof(int), sizeof(tPosteo), compararIds);

    metricas->hits = 0;
    metricas->misses = 0;
    metricas->lecturasDisco = 0;
    metricas->alturaArbol = 0;
    cantUsuarios = 0;

    inicio = clock();

    while (fread(&user, sizeof(tUsuario), 1, pfUsuarios) == 1)
    {
        cantUsuarios++;
        pFeed = user.feed;
        pFinFeed = user.feed + CANT_FEED_USUARIO;

        while (pFeed < pFinFeed)
        {
            id = *pFeed;

            /* 1. Intentamos obtener de la cache */
            if (obtenerLruCacheListaDobleyArbol(&cache, &id, &posteo))
            {
                metricas->hits++;
            }
            else
            {
                /* 2. Fallo de cache: Leemos de disco e insertamos en cache */
                metricas->misses++;
                if (id >= 0 && id < TOTAL_POSTEOS)
                {
                    fseek(pfPosteos, (long)id * (long)sizeof(tPosteo), SEEK_SET);
                    fread(&posteo, sizeof(tPosteo), 1, pfPosteos);
                    metricas->lecturasDisco++;

                    agregarLruCacheListaDobleyArbol(&cache, &id, &posteo);
                }
            }
            pFeed++;
        }
    }

    fin = clock();

    /* Consultamos la altura del indice antes de vaciar */
    metricas->alturaArbol = alturaIndiceLruCacheListaDobleyArbol(&cache);

    vaciarLruCacheListaDobleyArbol(&cache);
    fclose(pfPosteos);
    fclose(pfUsuarios);

    metricas->tiempoTotal = (double)(fin - inicio) / CLOCKS_PER_SEC;
    totalConsultas = cantUsuarios * CANT_FEED_USUARIO;

    metricas->tiempoPorUsuario = (cantUsuarios > 0) ? (metricas->tiempoTotal / cantUsuarios) : 0.0;
    metricas->tiempoPorPosteo = (totalConsultas > 0) ? (metricas->tiempoTotal / totalConsultas) : 0.0;

    return 1;
}

/* ========================================================================= */
/* UTILIDAD PARA IMPRIMIR METRICAS                                           */
/* ========================================================================= */
void imprimirReporte(const char *titulo, const tMetricasSimulacion *metricas, int totalUsuarios)
{
    int totalConsultas;
    double hitRate;

    totalConsultas = totalUsuarios * CANT_FEED_USUARIO;
    hitRate = (totalConsultas > 0) ? ((double)metricas->hits / totalConsultas * 100.0) : 0.0;

    printf("----------------------------------------------------------------------\n");
    printf("%s\n", titulo);
    printf("----------------------------------------------------------------------\n");
    printf("  Tiempo Total          : %8.4f seg\n", metricas->tiempoTotal);
    printf("  Tiempo Prom. / Usuario: %8.6f seg\n", metricas->tiempoPorUsuario);
    printf("  Tiempo Prom. / Posteo : %8.6f seg\n", metricas->tiempoPorPosteo);
    printf("  Hits (Aciertos)       : %8d (%6.2f %%)\n", metricas->hits, hitRate);
    printf("  Misses (Fallos)       : %8d\n", metricas->misses);
    printf("  Lecturas a Disco      : %8d\n", metricas->lecturasDisco);
    if (metricas->alturaArbol > 0)
    {
        printf("  Altura del Arbol ABB  : %8d\n", metricas->alturaArbol);
    }
    printf("----------------------------------------------------------------------\n\n");
}
