#include "tp1.h"
#include <stdlib.h>
#include <stdio.h>
#include "split.h"
#include <string.h>
#define TAMANIO_INICIAL 101
#define CANT_DATOS 5
#define SEPARADOR ','
#define CANTIDAD_TIPOS 4
#define CANT_TIPOS 8

typedef struct tp1 {
	struct pokemon **pokemones;
	size_t cantidad;
} tp1_t;

const char *NOMBRES_TIPOS[] = { "ELEC", "FUEG", "PLAN", "AGUA",
				"NORM", "FANT", "PSI",	"LUCH" };

char *leer_linea(FILE *archivo, bool *error_memoria, bool *termino_el_archivo)
{
	if (!archivo)
		return NULL;

	size_t tamanio_buffer = TAMANIO_INICIAL;

	char *buffer = malloc(tamanio_buffer * sizeof(char));
	if (!buffer)
		return NULL;

	size_t ocupado = 0;
	bool encontramos_salto_de_linea = false;

	while (!(*error_memoria) && !encontramos_salto_de_linea &&
	       fgets(buffer + ocupado, (int)(tamanio_buffer - ocupado),
		     archivo) != NULL) {
		ocupado += strlen(buffer + ocupado);

		if (buffer[ocupado - 1] == '\n') {
			encontramos_salto_de_linea = true;
		} else {
			char *buffer_aux = realloc(
				buffer, tamanio_buffer * 2 * sizeof(char));
			if (!buffer_aux) {
				*error_memoria = true;
			} else {
				buffer = buffer_aux;
				tamanio_buffer = tamanio_buffer * 2;
			}
		}
	}

	*termino_el_archivo =
		(!encontramos_salto_de_linea && ocupado == 0) ? true : false;

	if (*error_memoria || *termino_el_archivo) {
		free(buffer);
		return NULL;
	}
        
        if (encontramos_salto_de_linea) {
                buffer[ocupado - 1] = '\0';
                ocupado--; 
        }

	char *buffer_ajustado = realloc(buffer, ocupado+1);

	if (!buffer_ajustado)
		return buffer;

	return buffer_ajustado;
}

bool es_numero_valido(const char *cadena)
{
	if (!cadena || cadena[0] == '\0')
		return false;

	char *endptr;

	strtol(cadena, &endptr, 10);

	if (cadena == endptr)
		return false;

	if (*endptr != '\0')
		return false;

	return true;
}

bool formato_es_correcto(struct vector *v, int *tipo, int *metricas)
{
	if (!v)
		return false;

	bool son_correctos_nums = true;

	for (unsigned int i = 2; son_correctos_nums && i < v->cantidad; i++)
		if (es_numero_valido(v->palabras[i]))
			metricas[i - 2] = (int)strtol(v->palabras[i], NULL, 10);
		else
			son_correctos_nums = false;

	for (int i = 0; *tipo == -1 && i < CANT_TIPOS; i++) {
		if (strcmp(v->palabras[1], NOMBRES_TIPOS[i]) == 0)
			*tipo = i;
	}

	return (son_correctos_nums && *tipo != -1);
}

struct pokemon *parsear_linea(char *linea, bool *error_memoria)
{
	struct pokemon *pokemon_aux = malloc(sizeof(struct pokemon));
	if (!pokemon_aux) {
		*error_memoria = true;
		return NULL;
	}

	pokemon_aux->nombre = NULL;

	int tipo = -1;
	int metricas[3] = { -1, -1, -1 };

	struct vector *v = split(linea, SEPARADOR);

	if (!v) {
                free(pokemon_aux);
		*error_memoria = true;
	        return NULL;
        }

	if (v->cantidad != CANT_DATOS ||
	    !formato_es_correcto(v, &tipo, metricas)) {
		free(pokemon_aux);
		vector_destruir(v);
		return NULL;
	}

	char *nombre = malloc(strlen(v->palabras[0]) + 1);
	if (!nombre) {
		free(pokemon_aux);
		vector_destruir(v);
		*error_memoria = true;
		return NULL;
	}

	strcpy(nombre, v->palabras[0]);

	pokemon_aux->nombre = nombre;
	pokemon_aux->tipo = tipo;
	pokemon_aux->ataque = metricas[0];
	pokemon_aux->defensa = metricas[1];
	pokemon_aux->velocidad = metricas[2];

	vector_destruir(v);
	return pokemon_aux;
}

bool agregar_pokemon(tp1_t *tp, struct pokemon *pokemon_aux, bool *error_memoria)
{
	struct pokemon **pokemones_aux = realloc(
		tp->pokemones, (tp->cantidad + 1) * sizeof(struct pokemon *));

	if (!pokemones_aux){
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

void tp1_destruir(tp1_t *tp)
{
	for (size_t i = 0; i < tp->cantidad; i++) {
		free(tp->pokemones[i]->nombre);
                free(tp->pokemones[i]);
        }

	free(tp->pokemones);
	free(tp);
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

		if (pokemon_aux != NULL){
                        if (!agregar_pokemon(tp, pokemon_aux, &error_memoria))
                        {
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

	return tp;
}