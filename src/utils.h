#ifndef UTILS_H_
#define UTILS_H_
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct vector {
	size_t cantidad;
	char **palabras;
};

bool es_numero_valido(const char *cadena, int *valor);

struct vector *split(char *texto, char separador);

void vector_destruir(struct vector *vector);

char *leer_linea(FILE *archivo, bool *error_memoria, bool *termino_el_archivo);

void *ajustar_buffer(void *buffer, bool *error_memoria, size_t ocupado);

#endif //UTILS_H_