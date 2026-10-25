#include "TDA_LISTA/TDA_Lista.h"

int main()
{


    return 0;
}


//PRUEBA TDA_LISTA
//int main()
//{
//    tLista lista;
//    int valores[] = {10, 20, 30, 40};
//    int datoExtraido;
//
//    crearLista(&lista);
//    printf("¿Lista vacia al inicio? %s\n", listaVacia(&lista) ? "Si" : "No");
//
//    // Inserciones
//    ponerPrimeroLista(&lista, &valores[1], sizeof(int)); // Lista: 20
//    ponerPrimeroLista(&lista, &valores[0], sizeof(int)); // Lista: 10 -> 20
//    ponerUltimoLista(&lista, &valores[2], sizeof(int));  // Lista: 10 -> 20 -> 30
//    ponerUltimoLista(&lista, &valores[3], sizeof(int));  // Lista: 10 -> 20 -> 30 -> 40
//
//    printf("¿Lista vacia despues de insertar? %s\n", listaVacia(&lista) ? "Si" : "No");
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
//    printf("¿Lista vacia al final? %s\n", listaVacia(&lista) ? "Si" : "No");
//
//    return 0;
//}
