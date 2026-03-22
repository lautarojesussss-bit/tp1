#ifndef POKEMON_H_
#define POKEMON_H_
#include "tp1.h"
#include <stdio.h>
#include "split.h"
#include <stdbool.h>

char *leer_linea(FILE *archivo, bool *error_memoria, bool *termino_el_archivo);

bool es_numero_valido(const char *cadena);

bool formato_es_correcto(struct vector *v, int *tipo, int *metricas);

struct pokemon *parsear_linea(char *linea, bool *error_memoria);

#endif // POKEMON_H_