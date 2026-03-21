#include "tp1.h"
#include <stdio.h>
#define MAX_NOMBRE 200

typedef struct tp1 {
	struct pokemon **pokemones;
	size_t cantidad;
} tp1_t;

int main()
{
	char nombre_archivo[MAX_NOMBRE];
	scanf("%s", nombre_archivo);

	tp1_t *tp = tp1_leer_archivo(nombre_archivo);

	if (!tp) {
		printf("Error al intentar la función tp1_leer_archivo");
		return -1;
	}

	for (int i = 0; i < tp->cantidad; i++) {
		printf("%s\n", tp->pokemones[i]->nombre);
		printf("%i\n", tp->pokemones[i]->tipo);
		printf("%i\n", tp->pokemones[i]->ataque);
		printf("%i\n", tp->pokemones[i]->defensa);
		printf("%i\n\n", tp->pokemones[i]->velocidad);
	}

	return 0;
}