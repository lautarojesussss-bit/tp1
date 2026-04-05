#include "utils.h"
#include "constantes.h"

bool es_numero_valido(const char *cadena, int *valor)
{
	if (!cadena || cadena[0] == '\0')
		return false;

	char *endptr;
	long valor_convertido = 0;

	valor_convertido = strtol(cadena, &endptr, 10);

	if (cadena == endptr || *endptr != '\0')
		return false;

	*valor = (int)valor_convertido;

	return true;
}

void vector_destruir(struct vector *vector)
{
	if (!vector)
		return;

	size_t cant_palabras = vector->cantidad;

	for (size_t i = 0; i < cant_palabras; i++)
		free(vector->palabras[i]);

	free(vector->palabras);
	free(vector);
}

bool cargar_subpalabra(struct vector *vector_destino, char *fuente,
		       size_t pos_inicio, size_t pos_final)
{
	if (!vector_destino || !(vector_destino->palabras) || !fuente ||
	    pos_final < pos_inicio)
		return false;

	size_t largo_subpalabra = pos_final - pos_inicio;

	char *subpalabra_aux = malloc((largo_subpalabra + 1) * sizeof(char));

	if (!subpalabra_aux)
		return false;

	memcpy(subpalabra_aux, fuente + pos_inicio, largo_subpalabra);

	subpalabra_aux[largo_subpalabra] = '\0';

	vector_destino->palabras[vector_destino->cantidad] = subpalabra_aux;
	vector_destino->cantidad++;

	return true;
}

struct vector *crear_vector_para_n_palabras(size_t n_palabras_iniciales)
{
	struct vector *nuevo_vector = calloc(1, sizeof(struct vector));

	if (!nuevo_vector)
		return NULL;

	char **palabras_aux = calloc(n_palabras_iniciales, sizeof(char *));

	if (!palabras_aux) {
		free(nuevo_vector);
		return NULL;
	}

	nuevo_vector->palabras = palabras_aux;

	return nuevo_vector;
}

/*
 *PRE: texto debe ser un string
 *POST: se devuelve un puntero a una instancia de struct vector que tiene las
 *palabras, ya separadas,
 *y su tamaño en el campo cantidad, si falla algo se devuelve NULL y se imprime
 *un aviso
 */
struct vector *split(char *texto, char caracter_separador)
{
	if (!texto)
		return NULL;

	bool salir = false;
	size_t largo_texto = strlen(texto);
	size_t pos_inicio_palabra = 0;
	size_t pos_final_palabra = 0;

	struct vector *vector_resultado =
		crear_vector_para_n_palabras(largo_texto + 1);

	if (!vector_resultado)
		return NULL;

	for (size_t i = 0; !salir && i <= largo_texto; i++) {
		if (texto[i] == caracter_separador || i == largo_texto) {
			pos_final_palabra = i;

			if (!cargar_subpalabra(vector_resultado, texto,
					       pos_inicio_palabra,
					       pos_final_palabra))
				salir = true;

			pos_inicio_palabra = pos_final_palabra + 1;
		}
	}

	if (salir) {
		vector_destruir(vector_resultado);
		return NULL;
	}

	char **buffer_ajustado =
		realloc(vector_resultado->palabras,
			vector_resultado->cantidad * sizeof(char *));

	if (!buffer_ajustado) {
		vector_destruir(vector_resultado);
		return NULL;
	}

	vector_resultado->palabras = buffer_ajustado;
	return vector_resultado;
}

void *ajustar_buffer(void *buffer, bool *error_memoria, size_t ocupado)
{
	void *buffer_ajustado = realloc(buffer, ocupado);

	if (!buffer_ajustado) {
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
	if (!buffer) {
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
			buffer[ocupado - 1] = '\0';
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

	char *buffer_ajustado = ajustar_buffer(buffer, error_memoria,
					       (ocupado + 1) * sizeof(char));

	return buffer_ajustado;
}