#ifndef REVISAR_H_
#define REVISAR_H_
#include "tp1.h"
#include "struct_tp1.h"

bool es_el_mismo_pokemon(struct pokemon *pokemon_1, struct pokemon *pokemon_2);

bool son_iguales_los_tp(tp1_t *tp1, tp1_t *tp2);

void cargar_pokemon(struct pokemon *p, char *nombre, enum tipo_pokemon tipo ,int ataque, 
        int defensa, int velocidad);

#endif