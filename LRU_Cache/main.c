//#include "TDA_LISTA/TDA_Lista.h"
//
//int main()
//{
//
//
//    return 0;
//}

///PRUEBA GEMINI de las primitivas, habr�a que probarlo con archivos como dijo el profe
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Aseg�rate de incluir tus headers aqu�
#include "TDA_Cache_Lista_Simple/TDA_Cache_Lista.h"
#include "Utilitiarias.h"
#include "comun.h"

int main()
{
    tCache cache;
    int clave;
    char dato[20];
    char datoSalida[20];

    printf("1. PROBANDO CREAR CACHE\n");
    // Ahora pasamos cmpInt al momento de crear la cach�
    crearLruCache(&cache, 3, sizeof(int), sizeof(char) * 20, cmpInt);
    mostrarCache(&cache);

    printf("\n2. PROBANDO AGREGAR (Llenando la cache)\n");
    clave = 1; strcpy(dato, "Uno");
    agregarLruCache(&cache, &clave, dato); // Ya no pasamos cmpInt

    clave = 2; strcpy(dato, "Dos");
    agregarLruCache(&cache, &clave, dato);

    clave = 3; strcpy(dato, "Tres");
    agregarLruCache(&cache, &clave, dato);
    mostrarCache(&cache); // Deberia verse: 3, 2, 1

    printf("\n3. PROBANDO DESBORDE (Agregando un 4to elemento)\n");
    clave = 4; strcpy(dato, "Cuatro");
    agregarLruCache(&cache, &clave, dato);
    mostrarCache(&cache); // Se va el 1. Deberia verse: 4, 3, 2

    printf("\n4. PROBANDO OBTENER (Clave 2)\n");
    clave = 2;
    // Ya no pasamos cmpInt
    if (obtenerLruCache(&cache, &clave, datoSalida)) {
        printf("Obtenido exitosamente: %s\n", datoSalida);
    } else {
        printf("Error: No se encontro la clave %d\n", clave);
    }
    mostrarCache(&cache); // El 2 pasa al frente. Deberia verse: 2, 4, 3

    printf("\n5. PROBANDO ACTUALIZAR (Re-insertando clave 4)\n");
    clave = 4; strcpy(dato, "Cuatro-Mod");
    agregarLruCache(&cache, &clave, dato);
    mostrarCache(&cache); // El 4 se actualiza y pasa al frente. Deberia verse: 4, 2, 3

    printf("\n6. PROBANDO BORRAR (Clave 3)\n");
    clave = 3;
    // Ya no pasamos cmpInt
    if (borrarLruCache(&cache, &clave)) {
        printf("Clave %d borrada exitosamente.\n", clave);
    } else {
        printf("Error al borrar: clave %d no existe.\n", clave);
    }
    mostrarCache(&cache); // Deberia verse: 4, 2. Cantidad: 2/3

    printf("\n7. PROBANDO CASOS DE ERROR (Obtener y borrar algo que no existe)\n");
    clave = 99;
    if (!obtenerLruCache(&cache, &clave, datoSalida)) {
        printf("Correcto: No se pudo obtener la clave %d porque no existe.\n", clave);
    }
    if (!borrarLruCache(&cache, &clave)) {
        printf("Correcto: No se pudo borrar la clave %d porque no existe.\n", clave);
    }

    printf("\n8. PROBANDO VACIAR CACHE\n");
    vaciarLruCache(&cache);
    mostrarCache(&cache); // Deberia verse vacia, cantidad 0/3

    return 0;
}
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
