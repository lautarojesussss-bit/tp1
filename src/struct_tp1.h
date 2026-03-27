#ifndef STRUCT_TP1_H_
#define STRUCT_TP1_H
#include "tp1.h"

typedef struct tp1 {
	struct pokemon **pokemones_nombre;
        struct pokemon **pokemones_tipo[8];
	size_t cantidad_total;
        bool es_duenio;
        size_t cant_tipos[8];
} tp1_t;

#endif //STRUCT_TP1_H