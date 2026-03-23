#ifndef REVISAR_H_
#define REVISAR_H_
#include "tp1.h"

typedef struct tp1 {
	struct pokemon **pokemones;
	size_t cantidad;
} tp1_t;

bool es_el_mismo_pokemon(struct pokemon *pokemon_1, struct pokemon *pokemon_2);

bool son_iguales_los_tp(tp1_t *tp1, tp1_t *tp2);

void imprimir_pokemon(struct pokemon *pokemon);

void imprimir_tp(tp1_t *tp);


void cargar_pokemon(struct pokemon *p, char *nombre, enum tipo_pokemon tipo ,int ataque, 
        int defensa, int velocidad);

#endif