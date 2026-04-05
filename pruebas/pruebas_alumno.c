#include "../src/tp1.h"
#include <stdio.h>
#include <stdbool.h>
#include <strings.h>
#include <string.h>
#include "pa2m.h"
#include "../src/constantes.h"

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

void inicializar_escenario_estandar(
	struct pokemon pokemones[CANT_POKEMONES_PRUEBA],
	struct pokemon *punteros[CANT_POKEMONES_PRUEBA])
{
	cargar_pokemon(&pokemones[0], "Bulbasaur", TIPO_PLAN, 49, 49, 45);
	cargar_pokemon(&pokemones[1], "Charmander", TIPO_FUEG, 52, 43, 65);
	cargar_pokemon(&pokemones[2], "Jigglypuff", TIPO_NORM, 45, 20, 20);
	cargar_pokemon(&pokemones[3], "Pikachu", TIPO_ELEC, 55, 40, 90);
	cargar_pokemon(&pokemones[4], "Squirtle", TIPO_AGUA, 48, 65, 43);

	for (int i = 0; i < CANT_POKEMONES_PRUEBA; i++)
		punteros[i] = &pokemones[i];
}

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

bool es_el_tp_esperado(struct pokemon **pokemones, tp1_t *tpB)
{
	if (!pokemones || !tpB)
		return false;

	size_t cant = tp1_cantidad(tpB);

	bool son_iguales = true;

	for (int i = 0; son_iguales && i < cant; i++) {
		if (!es_el_mismo_pokemon(pokemones[i],
					 tp1_buscar_orden(tpB, i)))
			son_iguales = false;
	}

	return son_iguales;
}

bool son_el_mismo_tp(tp1_t *tpA, tp1_t *tpB)
{
	if (!tpA || !tpB)
		return false;

	size_t cant = tp1_cantidad(tpA);

	if (cant != tp1_cantidad(tpB))
		return false;

	bool son_iguales = true;

	for (int i = 0; son_iguales && i < cant; i++) {
		if (!es_el_mismo_pokemon(tp1_buscar_orden(tpA, i),
					 tp1_buscar_orden(tpB, i)))
			son_iguales = false;
	}

	return son_iguales;
}



void prueba_destruir_tp_vacio()
{
	bool se_rompe = true;

	const char *archivo = "pruebas/archivos_csv/vacio.csv";

	tp1_t *tp = tp1_leer_archivo(archivo);

	if (!tp) {
		printf("Se abortó esta prueba por fallo al reservar memoria. \n\n");
		return;
	}

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
		funciona && tp1_cantidad(tp) == 0,
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
		cant_leida = tp1_cantidad(tp);

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

	tp1_t *tp_aux = tp1_leer_archivo(ARCHIVO_UN_POKEMON);

	bool cumple = false;

	if (tp_aux != NULL) {
		cumple = tp1_cantidad(tp_aux) &&
			 es_el_tp_esperado(&dirrecion_correcta, tp_aux);
		tp1_destruir(tp_aux);
	}

	pa2m_afirmar(cumple, "Leer correctamente el pokemon %s del archivo %s",
		     pokemon_correcto.nombre, ARCHIVO_UN_POKEMON);
}

void prueba_pokemones_desordenados()
{
	struct pokemon pokemones[CANT_POKEMONES_PRUEBA];
	struct pokemon *punteros_pokemones[CANT_POKEMONES_PRUEBA];

	inicializar_escenario_estandar(pokemones, punteros_pokemones);

	tp1_t *tp_aux = NULL;
	bool cumple = false;

	const char *nombre_archivo =
		"pruebas/archivos_csv/pokemones_desordenados.csv";

	tp_aux = tp1_leer_archivo(nombre_archivo);

	if (tp_aux != NULL) {
		cumple = es_el_tp_esperado(punteros_pokemones, tp_aux);
		tp1_destruir(tp_aux);
	}

	pa2m_afirmar(
		cumple,
		"Leer correctamente pokemones de un archivo que los tiene desordenados");
}

void prueba_ignorar_lineas_vacias()
{
	struct pokemon pokemones[CANT_POKEMONES_PRUEBA];
	struct pokemon *punteros_pokemones[CANT_POKEMONES_PRUEBA];

	inicializar_escenario_estandar(pokemones, punteros_pokemones);

	tp1_t *tp_aux = NULL;
	bool cumple = false;

	const char *nombre_archivo =
		"pruebas/archivos_csv/pokemones_lineas_vacias.csv";

	tp_aux = tp1_leer_archivo(nombre_archivo);

	if (tp_aux != NULL) {
		cumple = tp1_cantidad(tp_aux) == CANT_POKEMONES_PRUEBA &&
			 es_el_tp_esperado(punteros_pokemones, tp_aux);
		tp1_destruir(tp_aux);
	}

	pa2m_afirmar(
		cumple,
		"Leer correctamente pokemones de un archivo que tiene lineas vacias");
}

void prueba_ignorar_pokemones_truchos()
{
	struct pokemon pokemones[CANT_POKEMONES_PRUEBA];
	struct pokemon *punteros_pokemones[CANT_POKEMONES_PRUEBA];

	inicializar_escenario_estandar(pokemones, punteros_pokemones);

	tp1_t *tp_aux = NULL;
	bool cumple = false;

	const char *nombre_archivo =
		"pruebas/archivos_csv/pokemones_tramposo.csv";

	tp_aux = tp1_leer_archivo(nombre_archivo);

	if (tp_aux != NULL) {
		cumple = tp1_cantidad(tp_aux) == 5 &&
			 es_el_tp_esperado(punteros_pokemones, tp_aux);
		tp1_destruir(tp_aux);
	}

	pa2m_afirmar(
		cumple,
		"Leer correctamente pokemones de un archivo que tiene algunos truchos");
}

void pruebas_filtrar_tipo_cant()
{
	tp1_t *tp_aux = NULL;
	tp1_t *tp_filtrado = NULL;
	bool cumple = false;

	const char *nombre_archivo =
		"pruebas/archivos_csv/pokemones_varios.csv";

	tp_aux = tp1_leer_archivo(nombre_archivo);

	if (tp_aux != NULL) {
		tp_filtrado = tp1_filtrar_tipo(tp_aux, TIPO_FUEG);
		cumple = tp1_cantidad(tp_filtrado) == 3;
		tp1_destruir(tp_filtrado);
		tp1_destruir(tp_aux);
	}

	pa2m_afirmar(
		cumple,
		"Filtrar correctamente la cantidad de pokemones de cierto tipo en un archivo.");
}

void filtrar_pokemones_correctamente()
{
	struct pokemon pokemon_correcto_1;
	struct pokemon pokemon_correcto_2;
	struct pokemon pokemon_correcto_3;

	cargar_pokemon(&pokemon_correcto_1, "Magmar", TIPO_FUEG, 95, 57, 93);
	cargar_pokemon(&pokemon_correcto_2, "Ponyta", TIPO_FUEG, 85, 55, -90);
	cargar_pokemon(&pokemon_correcto_3, "Vulpix", TIPO_FUEG, 41, 40, 65);

	struct pokemon *pokemones_aux[3] = { &pokemon_correcto_1,
					     &pokemon_correcto_2,
					     &pokemon_correcto_3 };

	const char *nombre_archivo =
		"pruebas/archivos_csv/pokemones_varios.csv";

	tp1_t *tp_aux = NULL;
	tp1_t *tp_filtrado = NULL;
	bool cumple = false;

	tp_aux = tp1_leer_archivo(nombre_archivo);

	if (tp_aux != NULL) {
		tp_filtrado = tp1_filtrar_tipo(tp_aux, TIPO_FUEG);
		cumple = (tp1_cantidad(tp_filtrado) == 3) &&
			 es_el_tp_esperado(pokemones_aux, tp_filtrado);
		//cant_leida = tp1_con_cada_pokemon(tp_filtrado, escribir_pokemon, stdout);
		tp1_destruir(tp_filtrado);
		tp1_destruir(tp_aux);
	}

	pa2m_afirmar(
		cumple,
		"Filtrar correctamente pokemones de cierto tipo en un archivo.");
}

void prueba_encontrar_pokemon_nombre()
{
	struct pokemon pokemon_correcto_1;
	cargar_pokemon(&pokemon_correcto_1, "Vulpix", TIPO_FUEG, 41, 40, 65);

	tp1_t *tp_aux = NULL;
	struct pokemon *buscado = NULL;
	bool cumple = false;
	const char *nombre_archivo =
		"pruebas/archivos_csv/pokemones_varios.csv";

	tp_aux = tp1_leer_archivo(nombre_archivo);

	if (tp_aux != NULL)
		buscado = tp1_buscar_nombre(tp_aux, "Vulpix");

	if (buscado != NULL)
		cumple = es_el_mismo_pokemon(&pokemon_correcto_1, buscado);

	pa2m_afirmar(cumple, "Encontrar correctamente un pokemón por nombre.");

	if (tp_aux != NULL)
		tp1_destruir(tp_aux);
}

void prueba_buscar_nombre_mal_escrito()
{
	struct pokemon pokemon_correcto_1;
	cargar_pokemon(&pokemon_correcto_1, "Vulpix", TIPO_FUEG, 41, 40, 65);

	tp1_t *tp_aux = NULL;
	struct pokemon *buscado = NULL;
	bool cumple = false;
	const char *nombre_archivo =
		"pruebas/archivos_csv/pokemones_varios.csv";

	tp_aux = tp1_leer_archivo(nombre_archivo);

	if (tp_aux != NULL)
		buscado = tp1_buscar_nombre(tp_aux, "VULpix");

	if (buscado != NULL)
		cumple = es_el_mismo_pokemon(&pokemon_correcto_1, buscado);

	pa2m_afirmar(
		cumple,
		"Encontrar correctamente un pokemón por nombre aunque esté mal escrito.");

	if (tp_aux != NULL)
		tp1_destruir(tp_aux);
}

void prueba_guardar_archivo()
{
	struct pokemon pokemones[CANT_POKEMONES_PRUEBA];
	struct pokemon *punteros_pokemones[CANT_POKEMONES_PRUEBA];

	inicializar_escenario_estandar(pokemones, punteros_pokemones);

	tp1_t *tp_aux = NULL;
	tp1_t *tp_releido = NULL;

	bool cumple = false;

	const char *nombre_archivo =
		"pruebas/archivos_csv/pokemones_desordenados.csv";
	const char *archivo_escribir =
		"pruebas/archivos_csv/pokemones_escritos.csv";

	tp_aux = tp1_leer_archivo(nombre_archivo);

	if (tp_aux != NULL && es_el_tp_esperado(punteros_pokemones, tp_aux)) {
		tp1_guardar_archivo(tp_aux, archivo_escribir);
		tp_releido = tp1_leer_archivo(archivo_escribir);
		cumple = son_el_mismo_tp(tp_aux, tp_releido);
		tp1_destruir(tp_aux);
		tp1_destruir(tp_releido);
	}

	pa2m_afirmar(
		cumple,
		"Leer correctamente pokemones de un archivo, escribirlo en otro archivo, y releerlos.");
}

void prueba_buscar_n()
{
	tp1_t *tp_aux = NULL;
	struct pokemon *pokemon_aux = NULL;
	struct pokemon pokemon_correcto_1;
	cargar_pokemon(&pokemon_correcto_1, "Bulbasaur", TIPO_PLAN, 49, 49, 45);
	const char *archivo = "pruebas/archivos_csv/pokemones_filtrados.csv";
	bool cumple = false;

	tp_aux = tp1_leer_archivo(archivo);
	if (tp_aux != NULL) {
		pokemon_aux = tp1_buscar_orden(tp_aux, 0);
		cumple = es_el_mismo_pokemon(pokemon_aux, &pokemon_correcto_1);
		tp1_destruir(tp_aux);
	}

	pa2m_afirmar(
		cumple,
		"Encontrar un pokemon en un tp1 mediante su posición por orden alfábetico.");
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
	prueba_buscar_nombre_mal_escrito();
}

void pruebas_escribir_archivo()
{
	prueba_guardar_archivo();
}

void pruebas_tp1_buscar_n()
{
	prueba_buscar_n();
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

	pa2m_nuevo_grupo("Pruebas de tp1_guardar_archivo");
	prueba_guardar_archivo();

	pa2m_nuevo_grupo("Pruebas de tp1_buscar_orden");
	pruebas_tp1_buscar_n();

	return pa2m_mostrar_reporte();
}