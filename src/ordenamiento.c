#include "ordenamiento.h"
#include "tp1.h"
#include <string.h>
#include <strings.h>
#include <stdlib.h>

void merge(struct pokemon **pokemones, int pos_inicio, int pos_mitad,
	   int pos_fin, bool *error_memoria)
{
	if (!pokemones || !error_memoria)
		return;

	int i = pos_inicio;
	int j = pos_mitad + 1;
	int k = 0;

	int tamanio = pos_fin - pos_inicio + 1;

	struct pokemon **pokemones_aux =
		malloc((size_t)(tamanio) * sizeof(struct pokemon *));

	if (!pokemones_aux) {
		*error_memoria = true;
		return;
	}

	while (i <= pos_mitad && j <= pos_fin) {
		if (strcasecmp(pokemones[i]->nombre, pokemones[j]->nombre) <=
		    0) {
			pokemones_aux[k] = pokemones[i];
			i++;
		} else {
			pokemones_aux[k] = pokemones[j];
			j++;
		}

		k++;
	}

	while (i <= pos_mitad) {
		pokemones_aux[k] = pokemones[i];
		i++;
		k++;
	}

	while (j <= pos_fin) {
		pokemones_aux[k] = pokemones[j];
		j++;
		k++;
	}

	for (k = 0; k < tamanio; k++)
		pokemones[pos_inicio + k] = pokemones_aux[k];

	free(pokemones_aux);
}

void merge_sort(struct pokemon **pokemones, int pos_inicio, int pos_fin,
		bool *error_memoria)
{
	if (!pokemones || !error_memoria)
		return;

	if (pos_fin > pos_inicio) {
		int pos_mitad = pos_inicio + (pos_fin - pos_inicio) / 2;
		merge_sort(pokemones, pos_inicio, pos_mitad, error_memoria);
		merge_sort(pokemones, pos_mitad + 1, pos_fin, error_memoria);

		if (!(*error_memoria))
			merge(pokemones, pos_inicio, pos_mitad, pos_fin,
			      error_memoria);
	}
}

void ordenar_alfabeticamente(struct pokemon **pokemones, bool *error_memoria,
			     size_t cantidad)
{
	if (!pokemones || !error_memoria)
		return;

	if (cantidad > 1)
		merge_sort(pokemones, 0, (int)(cantidad - 1), error_memoria);
}

void quitar_repetidos(struct pokemon ***pokemones, size_t *cantidad,
		      bool *error_memoria)
{
	if (!pokemones || !cantidad)
		return;

	size_t i = 1;
	size_t j = 0;

	size_t cant_original = *cantidad;

	while (i < cant_original && j < cant_original - 1) {
		if (strcasecmp(pokemones[0][i]->nombre,
			       pokemones[0][j]->nombre) == 0) {
			free(pokemones[0][i]->nombre);
			free(pokemones[0][i]);
		} else {
			pokemones[0][j + 1] = pokemones[0][i];
			j++;
		}
		i++;
	}

	*cantidad = (j + 1);

	struct pokemon **pokemones_ajustado =
		realloc(pokemones[0], (j + 1) * sizeof(struct pokemon *));

	if (!pokemones_ajustado) {
		*error_memoria = true;
		return;
	}

	pokemones[0] = pokemones_ajustado;
}
