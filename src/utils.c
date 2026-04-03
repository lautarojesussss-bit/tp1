#include "utils.h"
#define FORMATO_ESCRITURA "%s,%s,%i,%i,%i\n"

const char *NOMBRES_TIPOS[] = { "ELEC", "FUEG", "PLAN", "AGUA",
				"NORM", "FANT", "PSI",	"LUCH" };

bool escribir_pokemon(struct pokemon *pokemon, void *archivo)
{
	if (!pokemon || !archivo || !(pokemon->nombre))
		return false;

	fprintf(archivo, FORMATO_ESCRITURA, pokemon->nombre,
		NOMBRES_TIPOS[pokemon->tipo], pokemon->ataque, pokemon->defensa,
		pokemon->velocidad);

	return true;
}

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

void escribir_pokemones(tp1_t *tp1, FILE *archivo)
{
	if (!tp1 || !archivo)
		return;

	tp1_con_cada_pokemon(tp1, escribir_pokemon, archivo);
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