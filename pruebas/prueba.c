#include "tp1.h"
#include <stdio.h>
#include <stdbool.h>
#include <strings.h>
#include "revisar.h"
#include "pa2m.h"

#define MAX_NOMBRE 200

#define ARCHIVO_UN_POKEMON "prubas/archivos_csv/pokemones.csv"

#define NOMBRE_ARCHIVO_NUEVO "pruebas/archivos_csv/pokemones_filtrados.csv"

#define ARCHIVO_VARIOS_POKEMONES "pruebas/archivos_csv/5pokmones.csv"
#define CANT_LEIDA_ESPERADA 5

#define N_POKEMON 2
#define POKEMON_BUSCADO "Pikachu"

int main()
{
	tp1_t *tp = tp1_leer_archivo(ARCHIVO_UN_POKEMON);

	if (!tp) {
		printf("Error al intentar la función tp1_leer_archivo");
		return -1;
	}

	printf("Los %i pokemones leídos del archivo son:\n\n",
	       (int)(tp->cantidad));

	imprimir_tp(tp);

	if (tp1_guardar_archivo(tp, NOMBRE_ARCHIVO_NUEVO) != NULL)
		printf("Se cargaron correctamente los pokemones, ya filtrados y ordenados, en %s\n\n",
		       NOMBRE_ARCHIVO_NUEVO);

	struct pokemon *seguno_pokemon = tp1_buscar_orden(tp, N_POKEMON);

	if (seguno_pokemon != NULL) {
		printf("El pokemon número %i del tp es: \n\n", N_POKEMON);
		imprimir_pokemon(seguno_pokemon);
	}

	tp1_t *tp_fueg = tp1_filtrar_tipo(tp, TIPO_FUEG);

	if (tp_fueg != NULL) {
		printf("Los pokemones filtrados por tipo fuego son: \n\n");
		imprimir_tp(tp_fueg);
		tp1_destruir(tp_fueg);
	}

	struct pokemon *buscado = tp1_buscar_nombre(tp, POKEMON_BUSCADO);
	if (buscado != NULL) {
		printf("El pokemon %s buscado por nombre en el tp es: \n\n",
		       POKEMON_BUSCADO);
		imprimir_pokemon(buscado);
	}

	tp1_destruir(tp);

	return 0;
}

void prueba_tp1_leer_algo()
{
	tp1_t *tp = NULL;
	tp = tp1_leer_archivo(ARCHIVO_UN_POKEMON);

	pa2m_afirmar(
		tp != NULL,
		"Se logró leer algo a partir del archivo %s que tiene 1 pokemon con formato válido.",
		ARCHIVO_UN_POKEMON);
}

void prueba_tp1_leer_cantidad_correcta()
{
	tp1_t *tp = NULL;
	tp = tp1_leer_archivo(ARCHIVO_VARIOS_POKEMONES);

	pa2m_afirmar(tp->cantidad == CANT_LEIDA_ESPERADA,
		     "Se logró leer la cantidad esperada de pokemones.");
}

void prueba_tp1_leer_todo_bien()
{
	struct pokemon pokemon_correcto;

	cargar_pokemon(&pokemon_correcto, "Charmander", TIPO_FUEG, 49, 49, 45);

	struct pokemon *dirrecion_correcta = &pokemon_correcto;

	tp1_t tp_correcto;

	tp_correcto.cantidad = 1;
	tp_correcto.pokemones = &dirrecion_correcta;

	tp1_t *tp_aux = tp1_leer_archivo(ARCHIVO_UN_POKEMON);

	pa2m_afirmar(son_iguales_los_tp(&tp_correcto, tp_aux),
		     "Se logró leer correctamente el pokemon %s del archivo %s",
		     tp_correcto.pokemones[1]->nombre, ARCHIVO_UN_POKEMON);
}