//#include "TDA_LISTA/TDA_Lista.h"
//
//int main()
//{
//
//
//    return 0;
//}

///prueba gemini archivos
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "TDA_CACHE_OPTIMIZADA/TDA_CACHE_OPTIMIZADA.h"

typedef int tClave;
typedef struct {
    char info[20];
} tDato;

int compararClaves(const void *a, const void *b)
{
    tNodo2 *nodoA = *(tNodo2 **)a;
    tNodo2 *nodoB = *(tNodo2 **)b;
    tElemCacheListaDoble *elemA = (tElemCacheListaDoble *)nodoA->dato;
    tElemCacheListaDoble *elemB = (tElemCacheListaDoble *)nodoB->dato;
    return *(int *)elemA->clave - *(int *)elemB->clave;
}

// 1. Genera un archivo con 2 millones de claves aleatorias (pero repetidas)
void generarArchivoPrueba(const char *nombreArchivo, int cantidadLineas, int maxClave)
{
    FILE *pf = fopen(nombreArchivo, "wt");
    if (!pf) return;

    printf("Generando archivo de %d de lineas...\n", cantidadLineas);
    srand(time(NULL));
    for (int i = 0; i < cantidadLineas; i++)
    {
        // Genera claves entre 1 y maxClave para forzar colisiones y uso de caché
        int claveAleatoria = (rand() % maxClave) + 1;
        fprintf(pf, "%d\n", claveAleatoria);
    }
    fclose(pf);
    printf("Archivo generado exitosamente.\n\n");
}

int main()
{
    const char *nombreArchivo = "millones_datos.txt";
    int totalConsultas = 5000000;
    int clavesDistintas = 50000; // Total de datos posibles
    int capacidadCache = 10000;  // La caché solo puede guardar el 20% de los datos

    // Generar el archivo (se hace rápido)
    generarArchivoPrueba(nombreArchivo, totalConsultas, clavesDistintas);

    FILE *pf = fopen(nombreArchivo, "rt");
    if (!pf) {
        printf("Error al abrir el archivo.\n");
        return 1;
    }

    tCacheListaDoble miCache;
    crearLruCacheListaDobleyArbol(&miCache, capacidadCache, sizeof(tClave), sizeof(tDato), compararClaves);

    tClave claveActual;
    tDato datoActual;
    strcpy(datoActual.info, "Dato simulado");

    int hits = 0;
    int misses = 0;

    printf("Iniciando procesamiento de %d de consultas...\n", totalConsultas);

    // 2. Iniciamos el cronómetro
    clock_t inicio = clock();

    // 3. Leemos línea por línea y usamos la caché
    while (fscanf(pf, "%d", &claveActual) == 1)
    {
        tDato datoObtenido;

        // Intentamos obtenerlo de la caché (Hit)
        if (obtenerLruCacheListaDobleyArbol(&miCache, &claveActual, &datoObtenido))
        {
            hits++;
        }
        else // No estaba en caché (Miss) -> Simulamos ir a buscarlo y lo agregamos
        {
            misses++;
            agregarLruCacheListaDobleyArbol(&miCache, &claveActual, &datoActual);
        }
    }

    // 4. Frenamos el cronómetro
    clock_t fin = clock();
    double tiempoConsumido = (double)(fin - inicio) / CLOCKS_PER_SEC;

    fclose(pf);
    vaciarLruCacheListaDobleyArbol(&miCache);

    // 5. Imprimimos las estadísticas
    printf("==========================================\n");
    printf("RESULTADOS DEL BENCHMARK\n");
    printf("==========================================\n");
    printf("Tiempo total    : %.3f segundos\n", tiempoConsumido);
    printf("Aciertos (Hits) : %d\n", hits);
    printf("Fallos (Misses) : %d\n", misses);
    printf("Hit Rate        : %.2f %%\n", (float)hits / totalConsultas * 100);
    printf("==========================================\n");

    return 0;
}


//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include "TDA_LISTA_DOBLE/TDA_LISTA_DOBLE.h" // Ajustá la ruta según tu proyecto

// =========================================================================
// 1. DEFINIMOS LOS TIPOS DE DATOS DE PRUEBA
// =========================================================================
///prueba gemini lista doble:
//typedef int tClave; // Usaremos un entero simple como clave
//
//typedef struct {
//    char nombre[50];
//    int valor;
//} tDato; // Nuestro dato será un registro con un string y un número
//
//// =========================================================================
//// 2. FUNCIÓN DE COMPARACIÓN PARA EL ÁRBOL
//// =========================================================================
//// Recuerda que el árbol guarda (tNodo2 *), por ende recibe doble puntero
//int compararClaves(const void *a, const void *b)
//{
//    tNodo2 *nodoA = *(tNodo2 **)a;
//    tNodo2 *nodoB = *(tNodo2 **)b;
//
//    tElemCacheListaDoble *elemA = (tElemCacheListaDoble *)nodoA->dato;
//    tElemCacheListaDoble *elemB = (tElemCacheListaDoble *)nodoB->dato;
//
//    int claveA = *(int *)elemA->clave;
//    int claveB = *(int *)elemB->clave;
//
//    return claveA - claveB;
//}
//
//// =========================================================================
//// 3. FUNCIÓN PARA VISUALIZAR LA LÍNEA DE TIEMPO (LRU)
//// =========================================================================
//void imprimirCache(tCacheListaDoble *cache)
//{
//    printf("ESTADO DE LA CACHE (Mas reciente -> Mas viejo):\n");
//    tNodo2 *act = cache->listaDoble;
//
//    if (!act) {
//        printf("  [Cache Vacia]\n");
//    }
//
//    while (act) {
//        tElemCacheListaDoble *elem = (tElemCacheListaDoble *)act->dato;
//        int clave = *(int *)elem->clave;
//        tDato *dato = (tDato *)elem->dato;
//
//        printf("  [Clave: %d] -> %s (Valor: %d)\n", clave, dato->nombre, dato->valor);
//        act = act->sig;
//    }
//    printf("  (Elementos: %d / Capacidad Max: %d)\n", cache->ce, cache->cap);
//    printf("--------------------------------------------------\n");
//}
//
//// =========================================================================
//// 4. EL PROGRAMA PRINCIPAL
//// =========================================================================
//int main()
//{
//    tCacheListaDoble miCache;
//    tClave clave;
//    tDato dato;
//
//    // Inicializamos con capacidad MÁXIMA de 3 elementos
//    crearLruCacheListaDobleyArbol(&miCache, 3, sizeof(tClave), sizeof(tDato), compararClaves);
//
//    printf("--- 1. LLENANDO LA CACHE CON 3 ELEMENTOS ---\n");
//    clave = 10; strcpy(dato.nombre, "Diez"); dato.valor = 100;
//    agregarLruCacheListaDobleyArbol(&miCache, &clave, &dato);
//
//    clave = 20; strcpy(dato.nombre, "Veinte"); dato.valor = 200;
//    agregarLruCacheListaDobleyArbol(&miCache, &clave, &dato);
//
//    clave = 30; strcpy(dato.nombre, "Treinta"); dato.valor = 300;
//    agregarLruCacheListaDobleyArbol(&miCache, &clave, &dato);
//
//    // Debería imprimir: 30, luego 20, luego 10 (el 10 es el más viejo)
//    imprimirCache(&miCache);
//
//
//    printf("--- 2. OBTENER LA CLAVE 10 (Prueba de Hit LRU) ---\n");
//    printf("-> Al consultarla, la 10 deberia saltar al principio de la lista.\n");
//    tDato datoObtenido;
//    clave = 10;
//    if (obtenerLruCacheListaDobleyArbol(&miCache, &clave, &datoObtenido)) {
//        printf("EXITO: Se obtuvo '%s'.\n", datoObtenido.nombre);
//    }
//    // Debería imprimir: 10, luego 30, luego 20 (el 20 quedó como el más viejo)
//    imprimirCache(&miCache);
//
//
//    printf("--- 3. AGREGAR UN 4TO ELEMENTO (Prueba de Desborde) ---\n");
//    printf("-> La cache esta llena. Al meter el 40, debe eliminarse el 20.\n");
//    clave = 40; strcpy(dato.nombre, "Cuarenta"); dato.valor = 400;
//    agregarLruCacheListaDobleyArbol(&miCache, &clave, &dato);
//
//    // Debería imprimir: 40, 10, 30 (el 20 desapareció)
//    imprimirCache(&miCache);
//
//
//    printf("--- 4. BORRAR LA CLAVE 10 MANUALMENTE ---\n");
//    clave = 10;
//    borrarLruCacheListaDobleyArbol(&miCache, &clave);
//
//    // Debería quedar: 40, 30
//    imprimirCache(&miCache);
//
//
//    printf("--- 5. VACIAR LA CACHE ---\n");
//    vaciarLruCacheListaDobleyArbol(&miCache);
//    imprimirCache(&miCache);
//
//    return 0;
//}



///PRUEBA GEMINI de las primitivas con lista normal, habr�a que probarlo con archivos como dijo el profe
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//
//// Aseg�rate de incluir tus headers aqu�
//#include "TDA_Cache_Lista_Simple/TDA_Cache_Lista.h"
//#include "Utilitiarias.h"
//#include "comun.h"
//
//int main()
//{
//    tCache cache;
//    int clave;
//    char dato[20];
//    char datoSalida[20];
//
//    printf("1. PROBANDO CREAR CACHE\n");
//    // Ahora pasamos cmpInt al momento de crear la cach�
//    crearLruCache(&cache, 3, sizeof(int), sizeof(char) * 20, cmpInt);
//    mostrarCache(&cache);
//
//    printf("\n2. PROBANDO AGREGAR (Llenando la cache)\n");
//    clave = 1; strcpy(dato, "Uno");
//    agregarLruCache(&cache, &clave, dato); // Ya no pasamos cmpInt
//
//    clave = 2; strcpy(dato, "Dos");
//    agregarLruCache(&cache, &clave, dato);
//
//    clave = 3; strcpy(dato, "Tres");
//    agregarLruCache(&cache, &clave, dato);
//    mostrarCache(&cache); // Deberia verse: 3, 2, 1
//
//    printf("\n3. PROBANDO DESBORDE (Agregando un 4to elemento)\n");
//    clave = 4; strcpy(dato, "Cuatro");
//    agregarLruCache(&cache, &clave, dato);
//    mostrarCache(&cache); // Se va el 1. Deberia verse: 4, 3, 2
//
//    printf("\n4. PROBANDO OBTENER (Clave 2)\n");
//    clave = 2;
//    // Ya no pasamos cmpInt
//    if (obtenerLruCache(&cache, &clave, datoSalida)) {
//        printf("Obtenido exitosamente: %s\n", datoSalida);
//    } else {
//        printf("Error: No se encontro la clave %d\n", clave);
//    }
//    mostrarCache(&cache); // El 2 pasa al frente. Deberia verse: 2, 4, 3
//
//    printf("\n5. PROBANDO ACTUALIZAR (Re-insertando clave 4)\n");
//    clave = 4; strcpy(dato, "Cuatro-Mod");
//    agregarLruCache(&cache, &clave, dato);
//    mostrarCache(&cache); // El 4 se actualiza y pasa al frente. Deberia verse: 4, 2, 3
//
//    printf("\n6. PROBANDO BORRAR (Clave 3)\n");
//    clave = 3;
//    // Ya no pasamos cmpInt
//    if (borrarLruCache(&cache, &clave)) {
//        printf("Clave %d borrada exitosamente.\n", clave);
//    } else {
//        printf("Error al borrar: clave %d no existe.\n", clave);
//    }
//    mostrarCache(&cache); // Deberia verse: 4, 2. Cantidad: 2/3
//
//    printf("\n7. PROBANDO CASOS DE ERROR (Obtener y borrar algo que no existe)\n");
//    clave = 99;
//    if (!obtenerLruCache(&cache, &clave, datoSalida)) {
//        printf("Correcto: No se pudo obtener la clave %d porque no existe.\n", clave);
//    }
//    if (!borrarLruCache(&cache, &clave)) {
//        printf("Correcto: No se pudo borrar la clave %d porque no existe.\n", clave);
//    }
//
//    printf("\n8. PROBANDO VACIAR CACHE\n");
//    vaciarLruCache(&cache);
//    mostrarCache(&cache); // Deberia verse vacia, cantidad 0/3
//
//    return 0;
//}
//int main()
//{
//    return 0;
//}
//PRUEBA TDA_LISTA
//int main()
//{
//    tLista lista;
//    int valores[] = {10, 20, 30, 40};
//    int datoExtraido;
//
//    crearLista(&lista);
//    printf("�Lista vacia al inicio? %s\n", listaVacia(&lista) ? "Si" : "No");
//
//    // Inserciones
//    ponerPrimeroLista(&lista, &valores[1], sizeof(int)); // Lista: 20
//    ponerPrimeroLista(&lista, &valores[0], sizeof(int)); // Lista: 10 -> 20
//    ponerUltimoLista(&lista, &valores[2], sizeof(int));  // Lista: 10 -> 20 -> 30
//    ponerUltimoLista(&lista, &valores[3], sizeof(int));  // Lista: 10 -> 20 -> 30 -> 40
//
//    printf("�Lista vacia despues de insertar? %s\n", listaVacia(&lista) ? "Si" : "No");
//
//    // Lectura sin extraer
//    if(verPrimeroLista(&lista, &datoExtraido, sizeof(int)))
//        printf("Primer elemento visto: %d\n", datoExtraido); // Esperado: 10
//
//    if(verUltimoLista(&lista, &datoExtraido, sizeof(int)))
//        printf("Ultimo elemento visto: %d\n", datoExtraido); // Esperado: 40
//
//    // Extracciones
//    if(sacarPrimeroLista(&lista, &datoExtraido, sizeof(int)))
//        printf("Elemento sacado del inicio: %d\n", datoExtraido); // Saca el 10
//
//    if(sacarUltimoLista(&lista, &datoExtraido, sizeof(int)))
//        printf("Elemento sacado del final: %d\n", datoExtraido); // Saca el 40
//
//    // Vaciado final
//    vaciarLista(&lista);
//    printf("�Lista vacia al final? %s\n", listaVacia(&lista) ? "Si" : "No");
//
//    return 0;
//}
