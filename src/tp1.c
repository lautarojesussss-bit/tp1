#include "tp1.h"
#include <stdlib.h>
#include <stdio.h>
#include "split.h"
#include <string.h>
#include <strings.h>

#include "utils.h"

typedef struct tp1 {
	struct pokemon **pokemones_nombre;
        struct pokemon **pokemones_tipo[8];
	size_t cantidad_total;
        bool es_duenio;
        size_t cant_tipos[8];
} tp1_t;

#define TAMANIO_INICIAL 101
#define CANT_DATOS 5
#define SEPARADOR ','
#define CANT_TIPOS 8
#define FORMATO_ESCRITURA "%s,%s,%i,%i,%i\n"


/*
*PRE: pokemon_1 y pokemon_2 deben estar inicializados con strings en sus campos nombre
*POST: Devuelve 0 si tienen el mismo nombre los pokemones
*< 0 si el pokemon_1 va antes por orden alfabetico y >0 si 
*segundo va antes
*/


void merge_alfabetico(struct pokemon **pokemones, int pos_inicio, int pos_mitad,
		    int pos_fin, bool *error_memoria)
{
	if (!pokemones || !error_memoria)
		return;

	int i = pos_inicio;
	int j = pos_mitad + 1;
	int k = 0;

	int tamanio = pos_fin - pos_inicio + 1;

	struct pokemon **pokemones_aux =
		malloc((size_t)(tamanio) * sizeof(struct pokemon *));

	if (!pokemones_aux) {
		*error_memoria = true;
		return;
	}

	while (i <= pos_mitad && j <= pos_fin) {
		if (strcasecmp(pokemones[i]->nombre, pokemones[j]->nombre) <= 0) {
			pokemones_aux[k] = pokemones[i];
			i++;
		} else {
			pokemones_aux[k] = pokemones[j];
			j++;
		}

		k++;
	}

	while (i <= pos_mitad) {
		pokemones_aux[k] = pokemones[i];
		i++;
		k++;
	}

	while (j <= pos_fin) {
		pokemones_aux[k] = pokemones[j];
		j++;
		k++;
	}

	for (k = 0; k < tamanio; k++)
		pokemones[pos_inicio + k] = pokemones_aux[k];

	free(pokemones_aux);
}

void merge_sort_alfabetico(struct pokemon **pokemones, int pos_inicio,
			 int pos_fin, bool *error_memoria)
{
	if (!pokemones || !error_memoria)
		return;

	if (pos_fin > pos_inicio) {
		int pos_mitad = pos_inicio + (pos_fin - pos_inicio) / 2;
		merge_sort_alfabetico(pokemones, pos_inicio, pos_mitad,
				    error_memoria);
		merge_sort_alfabetico(pokemones, pos_mitad + 1, pos_fin,
				    error_memoria);

		if (!(*error_memoria))
			merge_alfabetico(pokemones, pos_inicio, pos_mitad,
				       pos_fin, error_memoria);
	}
}

void ordenar_alfabeticamente(struct pokemon **pokemones, bool *error_memoria,
			     size_t cantidad)
{
	if (!pokemones || !error_memoria || cantidad < 2)
		return;

	merge_sort_alfabetico(pokemones, 0, (int)(cantidad - 1),
				    error_memoria);
}

void limpiar_clasificar(struct pokemon ***pokemones, size_t *cantidad,
		      bool *error_memoria)
{
	if (!pokemones || !cantidad || !error_memoria || *cantidad < 2)
		return;

	size_t i = 1;
	size_t j = 0;

	size_t cant_original = *cantidad;

	while (i < cant_original && j < cant_original - 1) {
		if (strcasecmp(pokemones[0][i]->nombre,
			       pokemones[0][j]->nombre) == 0) {
			free(pokemones[0][i]->nombre);
			free(pokemones[0][i]);
		} else {
			pokemones[0][j + 1] = pokemones[0][i];
                        //acá debería agregar lo de que se actualiza el contador
                        //y se agrega el pokemon en donde corresponde 
			j++;
		}
		i++;
	}

	*cantidad = (j + 1);

	struct pokemon **pokemones_ajustado =
		realloc(pokemones[0], (j + 1) * sizeof(struct pokemon *));

	if (!pokemones_ajustado) {
		*error_memoria = true;
		return;
	}

	pokemones[0] = pokemones_ajustado;
}



void *ajustar_buffer_2(void* buffer, bool *error_memoria, size_t ocupado)
{
        void *buffer_ajustado = realloc(buffer, ocupado);

	if (!buffer_ajustado){
                *error_memoria = true;
                return NULL;
        }
	return buffer_ajustado;
}



char* ajustar_buffer(char* buffer, bool *error_memoria, size_t ocupado)
{
        char *buffer_ajustado = realloc(buffer, ocupado);

	if (!buffer_ajustado){
                *error_memoria = true;
                return NULL;
        }
	return buffer_ajustado;
}





char *leer_linea(FILE *archivo, bool *error_memoria, bool *termino_el_archivo)
{
	if (!archivo || !error_memoria || !termino_el_archivo)
		return NULL;

	size_t tamanio_buffer = TAMANIO_INICIAL;

	char *buffer = malloc(tamanio_buffer * sizeof(char));
	if (!buffer){
                *error_memoria = true;
                return NULL;
        }

	size_t ocupado = 0;
	bool encontramos_salto_de_linea = false;

	while (!(*error_memoria) && !encontramos_salto_de_linea &&
	       fgets(buffer + ocupado, (int)(tamanio_buffer - ocupado),
		     archivo) != NULL) {
		ocupado += strlen(buffer + ocupado);

		if (buffer[ocupado - 1] == '\n') {
			encontramos_salto_de_linea = true;
                        buffer[ocupado -1] = '\0';
                        ocupado--;
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

	*termino_el_archivo = (!encontramos_salto_de_linea && ocupado == 0);

	if (*error_memoria || *termino_el_archivo) {
		free(buffer);
		return NULL;
	}

        char* buffer_ajustado = ajustar_buffer_2(buffer, error_memoria, (ocupado +1)* sizeof(char));

        return buffer_ajustado;
}


bool formato_es_correcto(struct vector *v, int *tipo, int *metricas)
{
	if (!v)
		return false;

	bool son_correctos_nums = true;
        int valor_leido = 0;

	for (unsigned int i = 2; son_correctos_nums && i < v->cantidad; i++)
		if (es_numero_valido(v->palabras[i], &valor_leido))
			metricas[i - 2] = valor_leido;
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




bool agregar_pokemon_2(struct pokemon ***pokemones, struct pokemon *pokemon_aux,
		     bool *error_memoria, size_t *cantidad, size_t *tam_buffer)
{
        size_t nuevo_tam = 0; 

        if (*tam_buffer == *cantidad) {

                nuevo_tam = (*cantidad == 0) ? *tam_buffer : *tam_buffer*2;
                struct pokemon **pokemones_aux = realloc(
                        pokemones[0], nuevo_tam * sizeof(struct pokemon *));

                if (!pokemones_aux) {
                        *error_memoria = true;
                        return false;
                }

                pokemones[0] = pokemones_aux;
                *tam_buffer = nuevo_tam;
        }

	pokemones[0][*cantidad] = pokemon_aux;
	(*cantidad)++;

	return true;
}




bool agregar_pokemon(struct pokemon ***pokemones, struct pokemon *pokemon_aux,
		     bool *error_memoria, size_t *cantidad)
{
	struct pokemon **pokemones_aux = realloc(
		pokemones[0], ((*cantidad) + 1) * sizeof(struct pokemon *));

	if (!pokemones_aux) {
		*error_memoria = true;
		return false;
	}

	pokemones[0] = pokemones_aux;

	pokemones[0][*cantidad] = pokemon_aux;
	(*cantidad)++;

	return true;
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



tp1_t *tp1_crear(bool es_duenio)
{
	tp1_t *tp1_aux = calloc(1, sizeof(tp1_t));
	if (!tp1_aux)
		return NULL;

        tp1_aux->es_duenio = es_duenio;

	return tp1_aux;
}

void tp1_destruir(tp1_t *tp1)
{
        if (!tp1)
                return;

        size_t i = 0; 
        size_t cant = tp1->cantidad_total;
        bool se_borran_p = tp1->es_duenio;

        while ( se_borran_p && i < cant ) {
		free(tp1->pokemones_nombre[i]->nombre);
		free(tp1->pokemones_nombre[i]);
                i++;
	}

        for (size_t j = 0; j < CANT_TIPOS; j++)
                free(tp1->pokemones_tipo[j]);

	free(tp1->pokemones_nombre);
	free(tp1);
}


void limpiar_y_contar(tp1_t *tp,
		      bool *error_memoria)
{
	if (!tp || !error_memoria || tp->cantidad_total == 0)
		return;

	size_t i = 1;
	size_t j = 0;
        tp->cant_tipos[tp->pokemones_nombre[0]->tipo]++;

	size_t cant_original = tp->cantidad_total;

	while (i < cant_original && j < cant_original - 1) {

                struct pokemon* pokemon_aux = tp->pokemones_nombre[i];
                
		if (strcasecmp(pokemon_aux->nombre,
			       tp->pokemones_nombre[j]->nombre) == 0) {
			free(pokemon_aux->nombre);
			free(pokemon_aux);
		} else {
			tp->pokemones_nombre[j + 1] = tp->pokemones_nombre[i];
                        tp->cant_tipos[tp->pokemones_nombre[i]->tipo]++;
			j++;
		}
		i++;
	}

	tp->cantidad_total = (j + 1);

	struct pokemon **pokemones_ajustado =
		realloc(tp->pokemones_nombre, (j + 1) * sizeof(struct pokemon *));

	if (!pokemones_ajustado) {
		*error_memoria = true;
		return;
	}

	tp->pokemones_nombre = pokemones_ajustado;
        
}


void clasificar_por_tipo(tp1_t *tp, bool *error_memoria)
{
        for (size_t i = 0; !(*error_memoria) && i < CANT_TIPOS; i++) {
                tp->pokemones_tipo[i] = malloc(tp->cant_tipos[i] * sizeof(struct pokemon *));
                tp->cant_tipos[i] = 0;

                if (!(tp->pokemones_tipo[i])) 
                        *error_memoria = true;
        }

        if (*error_memoria)
                return;
        
        struct pokemon *pokemon_aux = NULL;
        size_t cant = tp->cantidad_total;

        for (size_t j = 0; j < cant; j++) {
                pokemon_aux = tp->pokemones_nombre[j];
                tp->pokemones_tipo[pokemon_aux->tipo][tp->cant_tipos[pokemon_aux->tipo]] = pokemon_aux;
                tp->cant_tipos[pokemon_aux->tipo]++;
        }
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

	tp1_t *tp = tp1_crear(true);

	if (!tp) {
                fclose(archivo);
		return NULL;
        }

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
			if (!agregar_pokemon(&(tp->pokemones_nombre), pokemon_aux,
					     &(error_memoria), &(tp->cantidad_total))) {
				free(pokemon_aux->nombre);
				free(pokemon_aux);
			}
		}
		pokemon_aux = NULL;
	}

	fclose(archivo);

        if (!error_memoria)
                ordenar_alfabeticamente(tp->pokemones_nombre, &error_memoria, tp->cantidad_total);
        
        if(!error_memoria)

                limpiar_y_contar(tp, &error_memoria);

        if (!error_memoria) {
                clasificar_por_tipo(tp, &error_memoria);
        }
        

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

	return tp1->cantidad_total;
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

	FILE *archivo = fopen(nombre, "w");
	if (!archivo)
		return NULL;

	escribir_pokemones(tp1, archivo);

	fclose(archivo);

	return tp1;
}

/**
* Devuelve el n-esimo pokemon por orden alfabetico (de menor a mayor).
* En caso de no encontrarlo devuelve NULL.
*/
struct pokemon *tp1_buscar_orden(tp1_t *tp, int n)
{
	if (n > tp->cantidad_total)
		return NULL;

	return tp->pokemones_nombre[n - 1];
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

	tp1_t *tp_aux = tp1_crear(false);

	if (!tp_aux)
		return NULL;
        
        size_t cant = un_tp->cant_tipos[tipo];

        struct pokemon **pokemenes_aux = malloc(cant * sizeof(struct pokemon*));

        if (!pokemenes_aux){
                tp1_destruir(tp_aux);
                return NULL;
        }

        struct pokemon **pokemones_tipo_aux = malloc(cant * sizeof(struct pokemon *));

        if (!pokemones_tipo_aux) {
                free(pokemenes_aux);
                tp1_destruir(tp_aux);
                return NULL;
        }

        memcpy(pokemenes_aux, un_tp->pokemones_tipo[tipo], cant * sizeof(struct pokemon *));

        memcpy(pokemones_tipo_aux, un_tp->pokemones_tipo[tipo], cant * sizeof(struct pokemon *));

        tp_aux->pokemones_nombre = pokemenes_aux;
        tp_aux->pokemones_tipo[tipo] = pokemones_tipo_aux;
        tp_aux->cantidad_total = cant;
        tp_aux->cant_tipos[tipo] = cant;

	return tp_aux;
}



struct pokemon *tp1_buscar_nombre(tp1_t *tp, const char *nombre)
{
	if (!tp || !nombre)
		return NULL;

	return busqueda(tp->pokemones_nombre, 0, (int)(tp->cantidad_total - 1), nombre);
}


/**
 * Aplica la función f a cada pokemon por orden alfabetico (de menor a mayor).
 * La función deja de aplicarse si f devuelve false o se terminaron los pokemones.
 * 
 * Devuelve la cantidad de pokemones sobre los cuales se aplicó la función f.
 */
size_t tp1_con_cada_pokemon(tp1_t *un_tp, bool (*f)(struct pokemon *, void *),
			    void *extra)
{
        bool seguimos = true;
        size_t i = 0;
        
        for (; seguimos && i < un_tp->cantidad_total ; i++)
                seguimos = f(un_tp->pokemones_nombre[i], extra);
        
        return i;
}

