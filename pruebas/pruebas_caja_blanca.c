#include "tp1.h"
#include <stdio.h>
#include <stdbool.h>
#include <strings.h>
#include "revisar.h"
#include "pa2m.h"
#include "pokemon.h"

#define MAX_NOMBRE 200

#define ARCHIVO_UN_POKEMON "pruebas/archivos_csv/un_pokemon.csv"

#define ARCHIVO_VACIO "pruebas/archivos_csv/vacio.csv"

#define NOMBRE_ARCHIVO_NUEVO "pruebas/archivos_csv/pokemones_filtrados.csv"

#define ARCHIVO_VARIOS_POKEMONES "pruebas/archivos_csv/5pokemones.csv"

#define CANT_LEIDA_ESPERADA 5

#define N_POKEMON 2
#define POKEMON_BUSCADO "Pikachu"

/* 

void prueba_destruir_tp_con_uno()
{
	tp1_t *tp = malloc(sizeof(tp1_t));

	if (!tp) {
		printf("Se abortó esta prueba por fallo al reservar memoria. \n\n");
		return;
	}


}
*/
void prueba_destruir_tp_vacio()
{
	bool se_rompe = true;
	tp1_t *tp = malloc(sizeof(tp1_t));

	if (!tp) {
		printf("Se abortó esta prueba por fallo al reservar memoria. \n\n");
		return;
	}

	tp->cantidad = 0;
	tp->pokemones = NULL;
	tp1_destruir(tp);
	se_rompe = false;

	pa2m_afirmar(
		!se_rompe,
		"Se lamma a tp1_destruir con un tp inicializado vacío y el programa no explota.");
}

void prueba_tp1_leer_archivo_vacio()
{
	tp1_t *tp = NULL;
	tp = tp1_leer_archivo(ARCHIVO_VACIO);

	bool funciona = tp != NULL;

	pa2m_afirmar(
		funciona && tp->cantidad == 0,
		"Leer un archivo vacío y que no se rompa el programa, que diga que la cantidad de pokemones del tp es cero.");
	if (funciona)
		tp1_destruir(tp);
}

void prueba_tp1_leer_algo()
{
	tp1_t *tp = NULL;
	tp = tp1_leer_archivo(ARCHIVO_UN_POKEMON);

	pa2m_afirmar(
		tp != NULL,
		"Leer algo a partir del archivo %s que tiene 1 pokemon con formato válido.",
		ARCHIVO_UN_POKEMON);

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

	pa2m_afirmar(cant_leida == CANT_LEIDA_ESPERADA,
		     "Leer la cantidad esperada de pokemones.");

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

	bool cumple = son_iguales_los_tp(&tp_correcto, tp_aux);

	pa2m_afirmar(cumple, "Leer correctamente el pokemon %s del archivo %s",
		     tp_correcto.pokemones[0]->nombre, ARCHIVO_UN_POKEMON);

	if (tp_aux != NULL)
		tp1_destruir(tp_aux);
}


void prueba_pokemones_desordenados()
{
	struct pokemon pokemon_correcto_1;
	struct pokemon pokemon_correcto_2;
	struct pokemon pokemon_correcto_3;
	struct pokemon pokemon_correcto_4;
	struct pokemon pokemon_correcto_5;

	cargar_pokemon(&pokemon_correcto_1, "Bulbasaur", TIPO_PLAN, 49, 49, 45);
	cargar_pokemon(&pokemon_correcto_2, "Charmander", TIPO_FUEG, 52, 43,
		       65);
	cargar_pokemon(&pokemon_correcto_3, "Jigglypuff", TIPO_NORM, 45, 20,
		       20);
	cargar_pokemon(&pokemon_correcto_4, "Pikachu", TIPO_ELEC, 55, 40, 90);
	cargar_pokemon(&pokemon_correcto_5, "Squirtle", TIPO_AGUA, 48, 65, 43);

	struct pokemon *pokemones_aux[5];

	pokemones_aux[0] = &pokemon_correcto_1;
	pokemones_aux[1] = &pokemon_correcto_2;
	pokemones_aux[2] = &pokemon_correcto_3;
	pokemones_aux[3] = &pokemon_correcto_4;
	pokemones_aux[4] = &pokemon_correcto_5;

	tp1_t tp_correcto;

	tp_correcto.cantidad = 5;
	tp_correcto.pokemones = pokemones_aux;

	tp1_t *tp_aux = NULL;

        const char * nombre_archivo = "pruebas/archivos_csv/pokemones_desordenados.csv";

	tp_aux =
		tp1_leer_archivo(nombre_archivo);

	bool cumple = son_iguales_los_tp(&tp_correcto, tp_aux);

	pa2m_afirmar(
		cumple,
		"Leer correctamente pokemones de un archivo que los tiene desordenados");

	if (tp_aux != NULL)
                tp1_destruir(tp_aux);
        
}

void prueba_ignorar_lineas_vacias()
{
	struct pokemon pokemon_correcto_1;
	struct pokemon pokemon_correcto_2;
	struct pokemon pokemon_correcto_3;
	struct pokemon pokemon_correcto_4;
	struct pokemon pokemon_correcto_5;

	cargar_pokemon(&pokemon_correcto_1, "Bulbasaur", TIPO_PLAN, 49, 49, 45);
	cargar_pokemon(&pokemon_correcto_2, "Charmander", TIPO_FUEG, 52, 43,
		       65);
	cargar_pokemon(&pokemon_correcto_3, "Jigglypuff", TIPO_NORM, 45, 20,
		       20);
	cargar_pokemon(&pokemon_correcto_4, "Pikachu", TIPO_ELEC, 55, 40, 90);
	cargar_pokemon(&pokemon_correcto_5, "Squirtle", TIPO_AGUA, 48, 65, 43);

	struct pokemon *pokemones_aux[5];

	pokemones_aux[0] = &pokemon_correcto_1;
	pokemones_aux[1] = &pokemon_correcto_2;
	pokemones_aux[2] = &pokemon_correcto_3;
	pokemones_aux[3] = &pokemon_correcto_4;
	pokemones_aux[4] = &pokemon_correcto_5;

	tp1_t tp_correcto;

	tp_correcto.cantidad = 5;
	tp_correcto.pokemones = pokemones_aux;

	tp1_t *tp_aux = NULL;

        const char * nombre_archivo = "pruebas/archivos_csv/pokemones_lineas_vacias.csv";

	tp_aux =
		tp1_leer_archivo(nombre_archivo);

	bool cumple = son_iguales_los_tp(&tp_correcto, tp_aux);

	pa2m_afirmar(
		cumple,
		"Leer correctamente pokemones de un archivo que tiene lineas vacias");

	if (tp_aux != NULL)
                tp1_destruir(tp_aux);
        
}



void prueba_ignorar_pokemones_truchos()
{
	struct pokemon pokemon_correcto_1;
	struct pokemon pokemon_correcto_2;
	struct pokemon pokemon_correcto_3;
	struct pokemon pokemon_correcto_4;
	struct pokemon pokemon_correcto_5;

	cargar_pokemon(&pokemon_correcto_1, "Bulbasaur", TIPO_PLAN, 49, 49, 45);
	cargar_pokemon(&pokemon_correcto_2, "Charmander", TIPO_FUEG, 52, 43,
		       65);
	cargar_pokemon(&pokemon_correcto_3, "Jigglypuff", TIPO_NORM, 45, 20,
		       20);
	cargar_pokemon(&pokemon_correcto_4, "Pikachu", TIPO_ELEC, 55, 40, 90);
	cargar_pokemon(&pokemon_correcto_5, "Squirtle", TIPO_AGUA, 48, 65, 43);

	struct pokemon *pokemones_aux[5];

	pokemones_aux[0] = &pokemon_correcto_1;
	pokemones_aux[1] = &pokemon_correcto_2;
	pokemones_aux[2] = &pokemon_correcto_3;
	pokemones_aux[3] = &pokemon_correcto_4;
	pokemones_aux[4] = &pokemon_correcto_5;

	tp1_t tp_correcto;

	tp_correcto.cantidad = 5;
	tp_correcto.pokemones = pokemones_aux;

	tp1_t *tp_aux = NULL;

        const char * nombre_archivo = "pruebas/archivos_csv/pokemones_tramposo.csv";

	tp_aux =
		tp1_leer_archivo(nombre_archivo);

	bool cumple = son_iguales_los_tp(&tp_correcto, tp_aux);

	pa2m_afirmar(
		cumple,
		"Leer correctamente pokemones de un archivo que tiene algunos truchos");

	if (tp_aux != NULL)
                tp1_destruir(tp_aux);
        
}


void pruebas_filtrar_tipo_cant()
{
	tp1_t *tp_aux = NULL;

        const char * nombre_archivo = "pruebas/archivos_csv/pokemones_varios.csv";

	tp_aux = tp1_leer_archivo(nombre_archivo);

        tp1_t *tp_filtrado = NULL;

        tp_filtrado = tp1_filtrar_tipo(tp_aux, TIPO_FUEG);

	bool cumple = false;

        if (tp_filtrado != NULL)
                cumple = tp_filtrado->cantidad == 3;

	pa2m_afirmar(
		cumple,
		"Filtrar correctamente la cantidad de pokemones de cierto tipo en un archivo.");
        
        if (tp_filtrado != NULL)
                tp1_destruir(tp_filtrado);

	if (tp_aux != NULL)
                tp1_destruir(tp_aux);        
}

void filtrar_pokemones_correctamente()
{
	struct pokemon pokemon_correcto_1;
	struct pokemon pokemon_correcto_2;
	struct pokemon pokemon_correcto_3;

	cargar_pokemon(&pokemon_correcto_1, "Magmar", TIPO_FUEG, 95, 57, 93);
	cargar_pokemon(&pokemon_correcto_2, "Ponyta", TIPO_FUEG, 85, 55,
		       90);
	cargar_pokemon(&pokemon_correcto_3, "Vulpix", TIPO_FUEG, 41, 40,
		       65);

	struct pokemon *pokemones_aux[3];

	pokemones_aux[0] = &pokemon_correcto_1;
	pokemones_aux[1] = &pokemon_correcto_2;
	pokemones_aux[2] = &pokemon_correcto_3;

	tp1_t tp_correcto;

	tp_correcto.cantidad = 3;
	tp_correcto.pokemones = pokemones_aux;

        tp1_t *tp_aux = NULL;

        const char * nombre_archivo = "pruebas/archivos_csv/pokemones_varios.csv";

	tp_aux = tp1_leer_archivo(nombre_archivo);

        tp1_t *tp_filtrado = NULL;

        tp_filtrado = tp1_filtrar_tipo(tp_aux, TIPO_FUEG);

	bool cumple = false;

        if (tp_filtrado != NULL)
                cumple = son_iguales_los_tp(&tp_correcto, tp_filtrado);

        size_t cant_leida = 0;
        
        if (tp_filtrado != NULL){
                cant_leida = tp1_con_cada_pokemon(tp_filtrado, escribir_pokemon, stdout);
                tp1_destruir(tp_filtrado);
                
        }

        pa2m_afirmar(
		cumple,
		"Filtrar correctamente pokemones %d de cierto tipo en un archivo.", cant_leida);

	if (tp_aux != NULL)
                tp1_destruir(tp_aux);          
}


void prueba_encontrar_pokemon_nombre()
{
        struct pokemon pokemon_correcto_1;
	cargar_pokemon(&pokemon_correcto_1, "Vulpix", TIPO_FUEG, 41, 40,
		       65);

        tp1_t *tp_aux = NULL;
        struct pokemon *buscado = NULL;
        bool cumple = false;
        const char * nombre_archivo = "pruebas/archivos_csv/pokemones_varios.csv";

	tp_aux = tp1_leer_archivo(nombre_archivo);

        if (tp_aux != NULL)
                buscado = tp1_buscar_nombre(tp_aux, "Vulpix");

        if (buscado != NULL)
                cumple = es_el_mismo_pokemon(&pokemon_correcto_1, buscado);

	pa2m_afirmar(
		cumple,
		"Encontrar correctamente un pokemón por nombre.");

	if (tp_aux != NULL)
                tp1_destruir(tp_aux);    
}

void pruebas_unitarias_tp1_destruir()
{
	prueba_destruir_tp_vacio();
}


void pruebas_unitarias_tp1_leer()
{
	prueba_tp1_leer_archivo_vacio();
	prueba_tp1_leer_algo();
	prueba_tp1_leer_cantidad_correcta();
	prueba_tp1_uno_bien();
        prueba_ignorar_pokemones_truchos();
        prueba_pokemones_desordenados();
        prueba_ignorar_lineas_vacias();
}

void pruebas_unitarias_filtrar()
{
        pruebas_filtrar_tipo_cant();
        filtrar_pokemones_correctamente();
}

void pruebas_unitarias_buscar_nombre()
{
        prueba_encontrar_pokemon_nombre();
}

int main()
{
	pa2m_nuevo_grupo("============== PRUEBAS DEL TP1 ===============");

	pa2m_nuevo_grupo("Pruebas de tp1_destruir");
	pruebas_unitarias_tp1_destruir();

	pa2m_nuevo_grupo("Pruebas de tp1_leer_archivo");
	pruebas_unitarias_tp1_leer();

	pa2m_nuevo_grupo("Pruebas de tp1_filtrar_tipo");
        pruebas_unitarias_filtrar();

	pa2m_nuevo_grupo("Pruebas de tp1_buscar_nombre");
        pruebas_unitarias_buscar_nombre();

	return pa2m_mostrar_reporte();
}