#include "tp1.h"
#include <stdio.h>
#include <stdbool.h>
#include <strings.h>
#include "revisar.h"
#include "pa2m.h"

#define MAX_NOMBRE 200

#define ARCHIVO_UN_POKEMON "pruebas/archivos_csv/un_pokemon.csv"

#define NOMBRE_ARCHIVO_NUEVO "pruebas/archivos_csv/pokemones_filtrados.csv"

#define ARCHIVO_VARIOS_POKEMONES "pruebas/archivos_csv/5pokemones.csv"

#define CANT_LEIDA_ESPERADA 5

#define N_POKEMON 2
#define POKEMON_BUSCADO "Pikachu"




void prueba_tp1_leer_algo()
{
        tp1_t *tp = NULL;
        tp = tp1_leer_archivo(ARCHIVO_UN_POKEMON);

        pa2m_afirmar(tp != NULL, "Leer algo a partir del archivo %s que tiene 1 pokemon con formato válido.", ARCHIVO_UN_POKEMON);

        if (tp != NULL)
                tp1_destruir(tp);
}

void prueba_tp1_leer_cantidad_correcta()
{
        tp1_t *tp = NULL;
        tp = tp1_leer_archivo(ARCHIVO_VARIOS_POKEMONES);

        size_t cant_leida = 0;
        if (tp != NULL)
                cant_leida = tp->cantidad;
        

        pa2m_afirmar(cant_leida == CANT_LEIDA_ESPERADA, "Leer la cantidad esperada de pokemones.");

        if (tp != NULL)
                tp1_destruir(tp);
}

void prueba_tp1_uno_bien()
{
        struct pokemon pokemon_correcto;

        cargar_pokemon(&pokemon_correcto, "Charmander", TIPO_FUEG, 52, 43, 65);

        struct pokemon *dirrecion_correcta = &pokemon_correcto; 

        tp1_t tp_correcto;

        tp_correcto.cantidad = 1;
        tp_correcto.pokemones = &dirrecion_correcta;

        tp1_t *tp_aux = NULL;
        
        tp_aux = tp1_leer_archivo(ARCHIVO_UN_POKEMON);

        bool cumple = son_iguales_los_tp(&tp_correcto,tp_aux);

        pa2m_afirmar(cumple, "Leer correctamente el pokemon %s del archivo %s", tp_correcto.pokemones[0]->nombre, ARCHIVO_UN_POKEMON);

        if (tp_aux != NULL)
                tp1_destruir(tp_aux);
                
}

void pruebas_unitarias_tp1_leer()
{
        prueba_tp1_leer_algo();
        prueba_tp1_leer_cantidad_correcta();
        prueba_tp1_uno_bien();
}

int main()
{
	pa2m_nuevo_grupo("============== PRUEBAS DEL TP1 ===============");

	pa2m_nuevo_grupo("Pruebas de tp1_leer_archivo");
	pruebas_unitarias_tp1_leer();

	return pa2m_mostrar_reporte();
}