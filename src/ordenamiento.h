#ifndef ORDENAMIENTO_H_
#define ORDENAMIENTO_H_
#include "tp1.h"

void ordenar_alfabeticamente(struct pokemon **pokemones, bool *error_memoria, size_t cantidad);

void quitar_repetidos(struct pokemon ***pokemones, size_t *cantidad, bool *error_memoria);

#endif // ORDENAMIENTO_H_