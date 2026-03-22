#include "tp1.h"
#include <stdio.h>
#define MAX_NOMBRE 200
#define NOMBRE_ARCHIVO "pokemones.csv"
#define NOMBRE_ARCHIVO_NUEVO "pokemones_filtrados.csv"

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

	tp1_destruir(tp);

	return 0;
}