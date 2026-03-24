#ifndef POKEMON_H_
#define POKEMON_H_
#include "tp1.h"
#include <stdio.h>
#include "split.h"
#include <stdbool.h>

extern const char *NOMBRES_TIPOS[];

char *leer_linea(FILE *archivo, bool *error_memoria, bool *termino_el_archivo);

bool es_numero_valido(const char *cadena);

bool formato_es_correcto(struct vector *v, int *tipo, int *metricas);

struct pokemon *parsear_linea(char *linea, bool *error_memoria);

bool escribir_pokemon(struct pokemon *pokemon, void *archivo);

void escribir_pokemones(struct pokemon **pokemones, FILE *archivo, size_t cant);

bool agregar_pokemon(struct pokemon ***pokemones, struct pokemon *pokemon_aux,
		     bool *error_memoria, size_t *cantidad);

struct pokemon *crear_copia_pokemon(struct pokemon *pokemon, bool *error_memoria);

struct pokemon *busqueda(struct pokemon **pokemones, int pos_inicio, int pos_fin, const char *nombre);

#endif // POKEMON_H_