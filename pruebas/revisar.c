#include "pokemon.h"
#include "tp1.h"
#include <stdlib.h>
#include <stdio.h>
#include "split.h"
#include <string.h>
#include <strings.h>

typedef struct tp1 {
	struct pokemon **pokemones;
	size_t cantidad;
} tp1_t;

bool es_el_mismo_pokemon(struct pokemon *pokemon_1, struct pokemon *pokemon_2)
{
	if (!pokemon_1 || !pokemon_2)
		return false;

	return (strcmp(pokemon_1->nombre, pokemon_2->nombre) == 0 &&
		pokemon_1->tipo == pokemon_2->tipo &&
		pokemon_1->ataque == pokemon_2->ataque &&
		pokemon_1->defensa == pokemon_2->defensa &&
		pokemon_1->velocidad == pokemon_2->velocidad);
}

bool son_iguales_los_tp(tp1_t *tp1, tp1_t *tp2)
{
	if (!tp1 || !tp2)
		return false;

	size_t cant = tp1->cantidad;

	if (cant != tp2->cantidad)
		return false;

	bool son_iguales = true;

	for (size_t i = 0; i < cant; i++) {
		if (!es_el_mismo_pokemon(tp1->pokemones[i], tp2->pokemones[i]))
			son_iguales = false;
	}

	return son_iguales;
}

void imprimir_pokemon(struct pokemon *pokemon)
{
	printf("%s,%s,%i,%i,%i\n", pokemon->nombre, NOMBRES_TIPOS[pokemon->tipo],
	       pokemon->ataque, pokemon->defensa, pokemon->velocidad);
}

void imprimir_tp(tp1_t *tp)
{
	for (int i = 0; i < tp->cantidad; i++)
		imprimir_pokemon(tp->pokemones[i]);
}

void cargar_pokemon(struct pokemon *p, char *nombre, enum tipo_pokemon tipo,
		    int ataque, int defensa, int velocidad)
{
	if (!nombre || !p)
		return;

	p->ataque = ataque;
	p->defensa = defensa;
	p->velocidad = velocidad;
	p->tipo = tipo;
	p->nombre = nombre;
}
