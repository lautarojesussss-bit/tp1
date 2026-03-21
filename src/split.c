#include "split.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ERROR_MENSAJE "Error al intentar reservar memoria.\n"

/*
 *PRE: vector debe haber sido creado con vector_crear()
 *POST: libera toda la memoria reservada vinculada a v
 */
void vector_destruir(struct vector *v)
{
	if (!v)
		return;

	size_t len = v->cantidad;

	for (size_t i = 0; i < len; i++)
		free(v->palabras[i]);

	free(v->palabras);
	free(v);
}

/*
 *PRE: v debe haber sido creado con vector_inicializar(),
 *cant_caracteres_aux debe ser mayor o igual a 1.
 *POST: actualiza la última palabra de v con los datos pasados a la función
 */
bool actualizar_palabra(struct vector *v, size_t *cant_caracteres_aux,
			char nueva_letra)
{
	size_t pos_aux = (v->cantidad - 1);

	char *palabra_actualizar_aux =
		realloc(v->palabras[pos_aux],
			(*cant_caracteres_aux + 1) * (sizeof(char)));

	if (!palabra_actualizar_aux)
		return false;

	v->palabras[pos_aux] = palabra_actualizar_aux;
	v->palabras[pos_aux][*cant_caracteres_aux - 1] = nueva_letra;
	v->palabras[pos_aux][*cant_caracteres_aux] = '\0';
	(*cant_caracteres_aux)++;

	return true;
}

/*
 *PRE: v debe haber sido creado con vector_inicializar()
 *POST: agrega una nueva palabra a v y la inicializa
 * solo con el carácter nulo
 */
bool agrandar_vector_palabras(struct vector *v, size_t *cant_caracteres_aux)
{
	char **palabras_aux =
		realloc(v->palabras, (v->cantidad + 1) * sizeof(char *));

	if (!palabras_aux)
		return false;

	v->palabras = palabras_aux;
	v->palabras[v->cantidad] = NULL;
	(v->cantidad)++;

	char *nueva_palabra_aux = malloc(sizeof(char));

	if (!nueva_palabra_aux)
		return false;

	v->palabras[v->cantidad - 1] = nueva_palabra_aux;
	v->palabras[v->cantidad - 1][0] = '\0';
	*cant_caracteres_aux = 1;

	return true;
}

/*
 *PRE:
 *POST: devuelve un puntero a una instancia de struct vector inicializado
 *con una sola palabra que es solo el carácter nulo
 */
struct vector *vector_inicializar()
{
	struct vector *v = malloc(sizeof(struct vector));

	if (!v)
		return NULL;

	char **palabras_aux = malloc(sizeof(char *));

	if (!palabras_aux) {
		free(v);
		return NULL;
	}

	v->palabras = palabras_aux;

	char *caracter_inicial = malloc(sizeof(char));

	if (!caracter_inicial) {
		free(v->palabras);
		free(v);
		return NULL;
	}

	v->palabras[0] = caracter_inicial;
	v->palabras[0][0] = '\0';
	v->cantidad = 1;

	return v;
}

/*
 *PRE:
 *POST: devuelve NULL e imprime un aviso de error
 */
struct vector *aviso_error()
{
	printf(ERROR_MENSAJE);
	return NULL;
}

/*
 *PRE: texto debe ser un string
 *POST: se devuelve un puntero a una instancia de struct vector que tiene las
 *palabras, ya separadas,
 *y su tamaño en el campo cantidad, si falla algo se devuelve NULL y se imprime
 *un aviso
 */
struct vector *split(char *texto, char separador)
{
	if (!texto)
		return NULL;

	struct vector *resultado = vector_inicializar();

	if (!resultado)
		return aviso_error();

	size_t cant_caracteres_aux = 1;
	size_t len = strlen(texto);
	bool memoria_ok = true;

	for (size_t i = 0; memoria_ok && i < len; i++) {
		if (texto[i] == separador)
			memoria_ok = agrandar_vector_palabras(
				resultado, &cant_caracteres_aux);
		else
			memoria_ok = actualizar_palabra(
				resultado, &cant_caracteres_aux, texto[i]);
	}

	if (!memoria_ok) {
		vector_destruir(resultado);
		return aviso_error();
	}

	return resultado;
}
