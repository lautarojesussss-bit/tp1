#include "tp1.h"
#include <stdlib.h>
#include <stdio.h>
#include "split.h"
#include <string.h>
#include "pokemon.h"
#include "ordenamiento.h"

typedef struct tp1 {
	struct pokemon **pokemones;
	size_t cantidad;
} tp1_t;


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
			if (!agregar_pokemon(&(tp->pokemones), pokemon_aux, 
                                &error_memoria, &(tp->cantidad))) {
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
*/

tp1_t *tp1_guardar_archivo(tp1_t *tp1, const char *nombre)
{
        if (!tp1 || !nombre)
                return NULL;
        
        FILE* archivo = fopen(nombre, "w");
        if (!archivo)
                return NULL;

        escribir_pokemones(tp1->pokemones, archivo, tp1->cantidad);

        fclose(archivo);

        return tp1;
}

/**
* Devuelve el n-esimo pokemon por orden alfabetico (de menor a mayor).
* En caso de no encontrarlo devuelve NULL.
*/
struct pokemon *tp1_buscar_orden(tp1_t *tp, int n)
{
        if (n > tp->cantidad)
                return NULL;

        return tp->pokemones[n-1];
}



struct pokemon *crear_copia_pokemon(struct pokemon *pokemon, bool *error_memoria)
{
        if (!pokemon)
                return NULL;

        struct pokemon *pokemon_aux = malloc(sizeof(struct pokemon));

        if (!pokemon_aux) {
                *error_memoria = true;
                return NULL;
        }

        char *nombre_aux = malloc((strlen(pokemon->nombre)+1)*sizeof(char));
        if (!nombre_aux) {
                *error_memoria = true;
                return NULL;
        }

        strcpy(nombre_aux, pokemon->nombre);

        *pokemon_aux = *pokemon;
        pokemon_aux->nombre = nombre_aux;

        return pokemon_aux;
}

/**
* Dado un tp1 y un tipo, devuelve otro tp1 conteniendo solamente los pokemons de dicho tipo.
*
* En caso de error devuelve NULL.
*/
tp1_t *tp1_filtrar_tipo(tp1_t *un_tp, enum tipo_pokemon tipo)
{
        if (!un_tp)
                return NULL;

        tp1_t *tp_aux = tp1_crear();

	if (!tp_aux)
		return NULL;
        
        bool error_memoria = false;
        struct pokemon *pokemon_aux = NULL;
        size_t cant =  un_tp->cantidad;

        for (size_t i = 0; !error_memoria && i < cant ; i++) {
                if (un_tp->pokemones[i]->tipo == tipo) {
                        pokemon_aux = crear_copia_pokemon(un_tp->pokemones[i], &error_memoria);

                        if (pokemon_aux != NULL){
                                if(!agregar_pokemon(&(tp_aux->pokemones), pokemon_aux, &error_memoria, 
                                                &(tp_aux->cantidad))){
                                        free(pokemon_aux->nombre);
                                        free(pokemon_aux);
                                }
                        }

                        pokemon_aux = NULL;
                }
        }

        if (error_memoria) {
                tp1_destruir(tp_aux);
                return NULL;
        }
        
        return tp_aux;
}
