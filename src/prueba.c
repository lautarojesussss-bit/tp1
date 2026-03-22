#include "tp1.h"
#include <stdio.h>
#define MAX_NOMBRE 200
#define NOMBRE_ARCHIVO "pokemones_filtrados.csv"
#define NOMBRE_ARCHIVO_NUEVO "pokemones_filtrados.csv"
#define N_POKEMON 2

typedef struct tp1 {
	struct pokemon **pokemones;
	size_t cantidad;
} tp1_t;

int main()
{
	tp1_t *tp = tp1_leer_archivo(NOMBRE_ARCHIVO);

	if (!tp) {
		printf("Error al intentar la función tp1_leer_archivo");
		return -1;
	}

	printf("Los %i pokemones leídos del archivo son:\n\n",
	       (int)(tp->cantidad));

	for (int i = 0; i < tp->cantidad; i++) {
		printf("%s,%i,%i,%i,%i\n\n", tp->pokemones[i]->nombre,
		       tp->pokemones[i]->tipo, tp->pokemones[i]->ataque,
		       tp->pokemones[i]->defensa, tp->pokemones[i]->velocidad);
	}

        if (tp1_guardar_archivo(tp, NOMBRE_ARCHIVO_NUEVO ) != NULL)
                printf("Se cargaron correctamente los pokemones, ya filtrados y ordenados, en %s\n\n", NOMBRE_ARCHIVO_NUEVO);

        struct pokemon *seguno_pokemon = tp1_buscar_orden(tp, N_POKEMON);

        if (seguno_pokemon != NULL)
                printf("El pokemon número %i del tp es: %s\n\n", N_POKEMON, seguno_pokemon->nombre);
        
        tp1_t *tp_fueg = tp1_filtrar_tipo(tp, TIPO_FUEG);
        
        if (tp_fueg != NULL) {
                printf("Los pokemones filtrados por tipo fuego son: \n\n");
                for (int i = 0; i < tp_fueg->cantidad; i++) {
	                printf("%s,%i,%i,%i,%i\n\n", tp_fueg->pokemones[i]->nombre,
		               tp_fueg->pokemones[i]->tipo, tp_fueg->pokemones[i]->ataque,
		                tp_fueg->pokemones[i]->defensa, tp_fueg->pokemones[i]->velocidad);
	                }
                tp1_destruir(tp_fueg);
        }
        

	tp1_destruir(tp);

	return 0;
}