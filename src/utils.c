#include "utils.h"
#define FORMATO_ESCRITURA "%s,%s,%i,%i,%i\n"
#include <stdio.h>

const char *NOMBRES_TIPOS[] = { "ELEC", "FUEG", "PLAN", "AGUA",
				"NORM", "FANT", "PSI",	"LUCH" };

bool escribir_pokemon(struct pokemon *pokemon, void *archivo)
{
        if(!pokemon || !archivo ||!(pokemon->nombre))
                return false;

        fprintf(archivo, FORMATO_ESCRITURA, pokemon->nombre,
        NOMBRES_TIPOS[pokemon->tipo], pokemon->ataque,
        pokemon->defensa, pokemon->velocidad);

        return true;
}   


bool es_numero_valido(const char *cadena, int *valor)
{
	if (!cadena || cadena[0] == '\0')
		return false;

	char *endptr;
        long valor_convertido = 0;

	valor_convertido = strtol(cadena, &endptr, 10);

	if (cadena == endptr || *endptr != '\0')
		return false;

        *valor = (int)valor_convertido;

	return true;
}