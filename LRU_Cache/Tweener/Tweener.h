#ifndef TWEENER_H_INCLUDED
#define TWEENER_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../TDA_CACHE_OPTIMIZADA/TDA_CACHE_OPTIMIZADA.h"
#include "../Utilitiarias.h"
#include "../comun.h"
typedef struct {
    int id;
    char texto[141];
} tPosteo;

typedef struct {
    char usuario[15];
    int feed[20];
} tUsuario;

void generarPosteos(const char *path, int cantPosteos);
void generarUsuarios(const char *path, unsigned int semilla, int tipoLote, int cantPosteos) ;
void ejecutarSimulacion(const char *pathUsuarios, const char *pathPosteos, int capacidadCache, int modo);


#endif // TWEENER_H_INCLUDED
