#ifndef COMUN_H_INCLUDED
#define COMUN_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef int (*Cmp)(const void *a, const void *b);
typedef void (*actualizar) (void*, const void*);
typedef int(*tAccion)( void *, void*);
#endif
