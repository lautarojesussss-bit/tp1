#ifndef UTILS_H_ 
#define UTILS_H_
#include <stdbool.h>
#include "tp1.h"
#include <stdlib.h>

extern const char* NOMBRES_TIPOS[];

bool escribir_pokemon(struct pokemon *pokemon, void *archivo);


bool es_numero_valido(const char *cadena, int *valor);

#endif