#ifndef UTILS_H_
#define UTILS_H_
#include <stdbool.h>
#include "tp1.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct vector {
	size_t cantidad;
	char **palabras;
};

extern const char *NOMBRES_TIPOS[];

bool escribir_pokemon(struct pokemon *pokemon, void *archivo);

bool es_numero_valido(const char *cadena, int *valor);

void escribir_pokemones(tp1_t *tp1, FILE *archivo);

struct vector *split(char *texto, char separador);

void vector_destruir(struct vector *vector);

#endif //UTILS_H_