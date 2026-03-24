#include "pokemon.h"
#include "tp1.h"
#include <stdlib.h>
#include <stdio.h>
#include "split.h"
#include <string.h>
#include <strings.h>

#define TAMANIO_INICIAL 101
#define CANT_DATOS 5
#define SEPARADOR ','
#define CANT_TIPOS 8
#define FORMATO_ESCRITURA "%s,%s,%i,%i,%i\n"

const char *NOMBRES_TIPOS[] = { "ELEC", "FUEG", "PLAN", "AGUA",
				"NORM", "FANT", "PSI",	"LUCH" };

char *leer_linea(FILE *archivo, bool *error_memoria, bool *termino_el_archivo)
{
	if (!archivo || !error_memoria || !termino_el_archivo)
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

	char *buffer_ajustado = realloc(buffer, ocupado + 1);

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

bool agregar_pokemon(struct pokemon ***pokemones, struct pokemon *pokemon_aux,
		     bool *error_memoria, size_t *cantidad)
{
	struct pokemon **pokemones_aux = realloc(
		pokemones[0], (*cantidad + 1) * sizeof(struct pokemon *));

	if (!pokemones_aux) {
		*error_memoria = true;
		return false;
	}

	pokemones[0] = pokemones_aux;
	pokemones[0][*cantidad] = pokemon_aux;
	(*cantidad)++;

	return true;
}

bool escribir_pokemon(struct pokemon *pokemon, void *archivo)
{
        if(!pokemon || !archivo ||!(pokemon->nombre))
                return false;

        fprintf(archivo, FORMATO_ESCRITURA, pokemon->nombre,
        NOMBRES_TIPOS[pokemon->tipo], pokemon->ataque,
        pokemon->defensa, pokemon->velocidad);

        return true;
}       



void escribir_pokemones(struct pokemon **pokemones, FILE *archivo, size_t cant)
{
	if (!pokemones || !archivo)
		return;

	for (size_t i = 0; i < cant; i++) 
                escribir_pokemon(pokemones[i], archivo);
}

struct pokemon *crear_copia_pokemon(struct pokemon *pokemon,
				    bool *error_memoria)
{
	if (!pokemon)
		return NULL;

	struct pokemon *pokemon_aux = malloc(sizeof(struct pokemon));

	if (!pokemon_aux) {
		*error_memoria = true;
		return NULL;
	}

	char *nombre_aux = malloc((strlen(pokemon->nombre) + 1) * sizeof(char));
	if (!nombre_aux) {
		*error_memoria = true;
		return NULL;
	}

	strcpy(nombre_aux, pokemon->nombre);

	*pokemon_aux = *pokemon;
	pokemon_aux->nombre = nombre_aux;

	return pokemon_aux;
}

struct pokemon *busqueda(struct pokemon **pokemones, int pos_inicio,
			 int pos_fin, const char *nombre)
{
	int centro = pos_inicio + ((pos_fin - pos_inicio) / 2);

	int comparacion = strcasecmp(nombre, pokemones[centro]->nombre);

	if (comparacion == 0)
		return pokemones[centro];

	if (pos_fin <= pos_inicio)
		return NULL;

	if (comparacion < 0)
		return busqueda(pokemones, pos_inicio, centro - 1, nombre);
	else
		return busqueda(pokemones, centro + 1, pos_fin, nombre);
}