#include "tp1.h"
#include <stdlib.h>
#include <stdio.h>
#include "split.h"
#include <string.h>
#include "pokemon.h"
#include "ordenamiento.h"
#define FORMATO_ESCRITURA "%s,%s,%i,%i,%i\n"


typedef struct tp1 {
	struct pokemon **pokemones;
	size_t cantidad;
} tp1_t;



bool agregar_pokemon(tp1_t *tp, struct pokemon *pokemon_aux,
		     bool *error_memoria)
{
	struct pokemon **pokemones_aux = realloc(
		tp->pokemones, (tp->cantidad + 1) * sizeof(struct pokemon *));

	if (!pokemones_aux) {
		*error_memoria = true;
		return false;
	}

	tp->pokemones = pokemones_aux;
	tp->pokemones[tp->cantidad] = pokemon_aux;
	(tp->cantidad)++;

	return true;
}

tp1_t *tp1_crear()
{
	tp1_t *tp1_aux = malloc(sizeof(tp1_t));
	if (!tp1_aux)
		return NULL;

	tp1_aux->cantidad = 0;
	tp1_aux->pokemones = NULL;

	return tp1_aux;
}

void tp1_destruir(tp1_t *tp1)
{
	for (size_t i = 0; i < tp1->cantidad; i++) {
		free(tp1->pokemones[i]->nombre);
		free(tp1->pokemones[i]);
	}

	free(tp1->pokemones);
	free(tp1);
}

/**
 * Lee el archivo indicado y devuelve la estructura tp1 con los pokemones.
 * En caso de error devuelve NULL.
 * Tengo que abrir un archivo, leerlo con cierto formato ()
*/
tp1_t *tp1_leer_archivo(const char *nombre)
{
	FILE *archivo = fopen(nombre, "r");
	if (!archivo)
		return NULL;

	tp1_t *tp = tp1_crear();

	if (!tp)
		return NULL;

	bool error_memoria = false;
	bool termino_el_archivo = false;
	struct pokemon *pokemon_aux = NULL;

	while (!error_memoria && !termino_el_archivo) {
		char *linea = leer_linea(archivo, &error_memoria,
					 &termino_el_archivo);

		if (linea != NULL)
			pokemon_aux = parsear_linea(linea, &error_memoria);

		free(linea);

		if (pokemon_aux != NULL) {
			if (!agregar_pokemon(tp, pokemon_aux, &error_memoria)) {
				free(pokemon_aux->nombre);
				free(pokemon_aux);
			}
		}
		pokemon_aux = NULL;
	}

	fclose(archivo);

	if (error_memoria) {
		tp1_destruir(tp);
		return NULL;
	}

        ordenar_alfabeticamente(tp->pokemones, &error_memoria, tp->cantidad);

	if (error_memoria) {
		tp1_destruir(tp);
		return NULL;
	}

        quitar_repetidos(&(tp->pokemones), &(tp->cantidad), &error_memoria);

        if (error_memoria) {
                tp1_destruir(tp);
                return NULL;
        }

	return tp;
}

size_t tp1_cantidad(tp1_t *tp1)
{
        if (!tp1)
                return 0;

        return tp1->cantidad;
}

/**
 * Guarda en el archivo indicado los pokemones contenidos en la estructura tp1 
 * de manera tal que tp1_leer_archivo pueda volver a leerlo correctamente.
 *
 * Devuelve NULL en caso de error o el tp1 pasado por parámetro en caso de exito.

tp1_t *tp1_guardar_archivo(tp1_t *tp1, const char *nombre)
{
        if (!tp1 || !nombre)
                return NULL;
        
        FILE* archivo = fopen(nombre, "w");
        if (!archivo)
                return NULL;
        
        
        
        
}*/