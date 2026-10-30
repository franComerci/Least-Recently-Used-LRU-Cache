#include "Tweener.h"

void generarPosteos(const char *path, int cantPosteos) {
    FILE *pf = fopen(path, "wb");
    if (!pf) {
        printf("Error creando archivo de posteos.\n");
        return;
    }

    tPosteo p;
    for (int i = 0; i < cantPosteos; i++) {
        p.id = i;
        snprintf(p.texto, sizeof(p.texto), "Este es el posteo numero %d de Tweener.", i);
        fwrite(&p, sizeof(tPosteo), 1, pf);
    }
    fclose(pf);
}

void generarUsuarios(const char *path, unsigned int semilla, int tipoLote, int cantPosteos) {
    FILE *pf = fopen(path, "wb");
    if (!pf) return;

    srand(semilla);
    tUsuario u;

    for (int i = 0; i < 200; i++) {
        snprintf(u.usuario, sizeof(u.usuario), "user_%03d", i);

        for (int j = 0; j < 20; j++) {
            if (tipoLote == 1) {
                // CONCENTRADO: Forzamos a que todos los usuarios consulten entre los primeros 100 posteos.
                // Esto garantiza un Hit Rate > 80% en la caché.
                int topeConcentrado = (cantPosteos < 100) ? cantPosteos : 100;
                u.feed[j] = rand() % topeConcentrado;
            }
            else if (tipoLote == 2) {
                // DISPERSO: Cualquier posteo de la base de datos
                u.feed[j] = rand() % cantPosteos;
            }
            else if (tipoLote == 3) {
                // ORDENADO: IDs siempre crecientes sin pasarse del límite
                int idBase = (i * 20 + j) * 5;
                u.feed[j] = (idBase < cantPosteos) ? idBase : (cantPosteos - 20 + j);
            }
        }
        fwrite(&u, sizeof(tUsuario), 1, pf);
    }
    fclose(pf);
}

#include <time.h>
int compararClaves(const void *a, const void *b)
{
    tNodo2 *nodoA = *(tNodo2 **)a;
    tNodo2 *nodoB = *(tNodo2 **)b;

    tElemCacheListaDoble *elemA = (tElemCacheListaDoble *)nodoA->dato;
    tElemCacheListaDoble *elemB = (tElemCacheListaDoble *)nodoB->dato;

    // ACÁ ESTÁ LA CLAVE: Desreferenciar el puntero para comparar los NÚMEROS, no las direcciones
    int claveA = *(int *)elemA->clave;
    int claveB = *(int *)elemB->clave;

    return claveA - claveB;
}

void ejecutarSimulacion(const char *pathUsuarios, const char *pathPosteos, int capacidadCache, int modo)
{
    FILE *fUsers = fopen(pathUsuarios, "rb");
    FILE *fPosts = fopen(pathPosteos, "rb");
    if (!fUsers || !fPosts) return;

    tCacheListaDoble cache;
    if (modo > 0) {
        crearLruCacheListaDobleyArbol(&cache, capacidadCache, sizeof(int), sizeof(tPosteo), compararClaves);
    }

    tUsuario u;
    tPosteo p;
    long hits = 0, misses = 0, lecturasDisco = 0;

    clock_t inicio = clock();

    while (fread(&u, sizeof(tUsuario), 1, fUsers) == 1) {
        for (int i = 0; i < 20; i++) {
            int idBuscado = u.feed[i];

            if (modo == 0) {
                // SIN CACHÉ: Acceso directo a disco siempre
                fseek(fPosts, idBuscado * sizeof(tPosteo), SEEK_SET);
                fread(&p, sizeof(tPosteo), 1, fPosts);
                lecturasDisco++;
            }
            else {
                // CON CACHÉ (Parte 1 o Parte 2)
                if (obtenerLruCacheListaDobleyArbol(&cache, &idBuscado, &p)) {
                    hits++;
                } else {
                    misses++;
                    // Lectura directa por posición relativa
                    fseek(fPosts, idBuscado * sizeof(tPosteo), SEEK_SET);
                    fread(&p, sizeof(tPosteo), 1, fPosts);
                    lecturasDisco++;

                    // Se agrega a la caché
                    agregarLruCacheListaDobleyArbol(&cache, &idBuscado, &p);
                }
            }
        }
    }

    clock_t fin = clock();
    double tiempoTotal = (double)(fin - inicio) / CLOCKS_PER_SEC;

    int totalPosteos = 200 * 20; // 4000 posteos procesados
    double tiempoPorUsuario = tiempoTotal / 200.0;
    double tiempoPorPosteo = tiempoTotal / (double)totalPosteos;

    printf("\n=== MODO: %s | Capacidad: %d ===\n",
           modo == 0 ? "Sin Cache" : (modo == 1 ? "Parte 1 (Sin ultNodo)" : "Parte 2 (Con ultNodo)"),
           capacidadCache);
    printf("Tiempo Total       : %.6f seg\n", tiempoTotal);
    printf("Tiempo x Usuario   : %.6f seg\n", tiempoPorUsuario);
    printf("Tiempo x Posteo    : %.6f seg\n", tiempoPorPosteo);

    if (modo > 0) {
        printf("Hits / Misses      : %ld / %ld\n", hits, misses);
        printf("Lecturas a Disco   : %ld\n", lecturasDisco);
        printf("Altura del Arbol   : %d\n", alturaArbol(&cache.arbol));
        vaciarLruCacheListaDobleyArbol(&cache);
    }

    fclose(fUsers);
    fclose(fPosts);
}
