#include "src/tp1.h"
#include <stdio.h>
#include <string.h>
#include "src/utils.h"
#include "src/constantes.h"

const char *NOMBRES_TIPOS[] = { "Eléctrico", "Fuego",	  "Planta",   "Agua",
				"Normal",    "Fantasmas", "Psíquico", "Lucha" };

/*
 * PRE: 'pokemon' debe ser un puntero válido. 'archivo' debe ser un puntero a un FILE abierto en modo escritura.
 * POST: Imprime los datos del pokemon por la salida indicada con el formato estándar. Devuelve true si tuvo éxito, false en caso contrario.
 */
bool imprimir_pokemon(struct pokemon *pokemon, void *archivo)
{
	if (!pokemon || !archivo || !(pokemon->nombre))
		return false;

	fprintf(archivo, COLOR_BLANCO FORMATO_IMPRESION COLOR_NORMAL,
		pokemon->nombre, NOMBRES_TIPOS[pokemon->tipo], pokemon->ataque,
		pokemon->defensa, pokemon->velocidad);

	return true;
}

/*
 * PRE: 'tp1' debe ser un tp1_t válido y 'archivo' un puntero a un FILE abierto en modo escritura (o el stdout).
 * POST: Utiliza el iterador interno del tp1 para imprimir todos los pokemones almacenados en él.
 */
void imprimir_pokemones(tp1_t *tp1, FILE *archivo)
{
	if (!tp1 || !archivo)
		return;

	tp1_con_cada_pokemon(tp1, imprimir_pokemon, archivo);
}

/*
 * PRE: 'tp' debe ser un tp1_t válido. 'error_memoria' inicializado en false.
 * POST: Filtra el TDA original iterando por todos los tipos posibles de pokemones. 
 * Imprime por pantalla todos los pokemones agrupados por tipo. En caso de fallo de memoria, actualiza la bandera.
 */
void imprimir_tp_por_tipo(tp1_t *tp, bool *error_memoria)
{
	if (!tp || !error_memoria || *error_memoria)
		return;

	tp1_t *tp_aux = NULL;

	for (size_t i = 0; !(*error_memoria) && i < CANT_TIPOS; i++) {
		tp_aux = tp1_filtrar_tipo(tp, i);

		if (!tp_aux) {
			*error_memoria = true;
		} else if (tp1_cantidad(tp_aux) > 0) {
			printf(COLOR_VERDE
			       "\n============================= POKEMONES TIPO %s ============================\n" COLOR_NORMAL,
			       NOMBRES_TIPOS[i]);
			imprimir_pokemones(tp_aux, stdout);
		}

		tp1_destruir(tp_aux);
	}
}

int main(int argc, char const *argv[])
{
	if (argc != CANT_ARGUMENTOS) {
		printf(COLOR_ROJO
		       "La cantidad de argumentos no es correcta.\n" COLOR_NORMAL);
		return ERROR;
	}

	bool es_accion_1 = strcmp(ACCION_1, argv[2]) == 0;
	bool es_accion_2 = strcmp(ACCION_2, argv[2]) == 0;

	if (!es_accion_1 && !es_accion_2) {
		printf(COLOR_ROJO
		       "La accion '%s' no está definida.\n" COLOR_NORMAL,
		       argv[2]);
		return ERROR;
	}

	tp1_t *tp = tp1_leer_archivo(argv[1]);

	if (!tp) {
		printf(COLOR_ROJO "Error al leer el archivo.\n" COLOR_NORMAL);
		return ERROR;
	}

	if (es_accion_1) {
		bool es_forma_1 = strcmp(argv[3], ACCION_1_FORMA_1) == 0;
		bool es_forma_2 = strcmp(argv[3], ACCION_1_FORMA_2) == 0;
		bool error_memoria = false;

		if (es_forma_1) {
			printf(COLOR_VERDE
			       "\n============================= TODOS LOS POKEMONES ============================\n" COLOR_NORMAL);
			imprimir_pokemones(tp, stdout);
		} else if (es_forma_2) {
			imprimir_tp_por_tipo(tp, &error_memoria);
		}
		if (error_memoria)
			printf(COLOR_ROJO
			       "Fallo al efectuar la operación por problemas con la memoria.\n" COLOR_NORMAL);
	} else {
		int posicion = -1;
		struct pokemon *pokemon_buscado = NULL;

		if (es_numero_valido(argv[3], &posicion)) {
			pokemon_buscado = tp1_buscar_orden(tp, posicion);
			if (!pokemon_buscado) {
				printf(COLOR_ROJO
				       "Error, el pokemon %d no se encontró.\n" COLOR_NORMAL,
				       posicion);
			} else {
				printf(COLOR_VERDE
				       "\n============================= POKÉMON EN LA POSICIÓN %i============================\n" COLOR_NORMAL,
				       posicion);
				imprimir_pokemon(pokemon_buscado, stdout);
			}
		} else {
			pokemon_buscado = tp1_buscar_nombre(tp, argv[3]);
			if (!pokemon_buscado) {
				printf(COLOR_ROJO
				       "Error el pokemon %s no se encontró.\n" COLOR_NORMAL,
				       argv[3]);
			} else {
				printf(COLOR_VERDE
				       "\n============================= EL POKÉMON DE NOMBRE %s ============================\n" COLOR_NORMAL,
				       argv[3]);
				imprimir_pokemon(pokemon_buscado, stdout);
			}
		}
	}

	tp1_destruir(tp);

	return 0;
}
