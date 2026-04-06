#include "../src/tp1.h"
#include <stdio.h>
#include <stdbool.h>
#include <strings.h>
#include <string.h>
#include "pa2m.h"
#include "../src/constantes.h"

bool es_un_pokemon(struct pokemon *pokemon, void *extra)
{
	if (!pokemon)
		return false;

	return true;
}

bool es_rapido(struct pokemon *p, void *extra)
{
	if (!p)
		return false;

	return p->velocidad > VELOCIDAD_MINIMA;
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
	const char *archivo = "pruebas/archivos_csv/vacio.csv";

	tp1_t *tp = tp1_leer_archivo(archivo);
	bool cumple = false;

	if (tp != NULL) {
		tp1_destruir(tp);
		cumple = true;
	}

	pa2m_afirmar(
		cumple,
		"Destruir un tp1 inicializado a partir de un archivo vacío no genera errores de memoria.");
}

void prueba_tp1_leer_archivo_vacio()
{
	const char *archivo = "pruebas/archivos_csv/vacio.csv";

	tp1_t *tp = tp1_leer_archivo(archivo);

	bool cumple = tp != NULL;

	pa2m_afirmar(
		cumple && tp1_cantidad(tp) == 0,
		"Leer un archivo vacío inicializa el tp1 correctamente con una cantidad de cero pokemones.");

	tp1_destruir(tp);
}

void prueba_tp1_leer_algo()
{
	tp1_t *tp = NULL;
	const char *archivo = "pruebas/archivos_csv/un_pokemon.csv";
	tp = tp1_leer_archivo(archivo);

	pa2m_afirmar(
		tp != NULL,
		"Leer un archivo con un único pokemon de formato válido crea el tp1 exitosamente.",
		archivo);

	tp1_destruir(tp);
}

void prueba_tp1_leer_cantidad_correcta()
{
	const char *archivo = "pruebas/archivos_csv/5pokemones.csv";
	tp1_t *tp = tp1_leer_archivo(archivo);

	size_t cant_leida = 0;
	if (tp != NULL)
		cant_leida = tp1_cantidad(tp);

	pa2m_afirmar(
		cant_leida == 5,
		"La cantidad de pokemones almacenados en el tp1 coincide con los registros válidos del archivo.");

	tp1_destruir(tp);
}

void prueba_tp1_uno_bien()
{
	struct pokemon pokemon_correcto;

	cargar_pokemon(&pokemon_correcto, "Charmander", TIPO_FUEG, 52, 43, 65);

	struct pokemon *direccion_correcta = &pokemon_correcto;

	const char *archivo = "pruebas/archivos_csv/un_pokemon.csv";

	tp1_t *tp_aux = tp1_leer_archivo(archivo);

	bool cumple = false;

	if (tp_aux != NULL) {
		cumple = tp1_cantidad(tp_aux) &&
			 es_el_tp_esperado(&direccion_correcta, tp_aux);
	}

	pa2m_afirmar(
		cumple,
		"Los atributos del pokemon leído desde el archivo %s coinciden exactamente con los esperados.",
		archivo);

	tp1_destruir(tp_aux);
}

void prueba_pokemones_desordenados()
{
	struct pokemon pokemones[CANT_POKEMONES_PRUEBA];
	struct pokemon *punteros_pokemones[CANT_POKEMONES_PRUEBA];

	inicializar_escenario_estandar(pokemones, punteros_pokemones);

	bool cumple = false;

	const char *nombre_archivo =
		"pruebas/archivos_csv/pokemones_desordenados.csv";

	tp1_t *tp_aux = tp1_leer_archivo(nombre_archivo);

	if (tp_aux != NULL) {
		cumple = es_el_tp_esperado(punteros_pokemones, tp_aux);
	}

	pa2m_afirmar(
		cumple,
		"Leer un archivo con registros desordenados los almacena correctamente ordenados alfabéticamente.");

	tp1_destruir(tp_aux);
}

void prueba_ignorar_lineas_vacias()
{
	struct pokemon pokemones[CANT_POKEMONES_PRUEBA];
	struct pokemon *punteros_pokemones[CANT_POKEMONES_PRUEBA];

	inicializar_escenario_estandar(pokemones, punteros_pokemones);

	bool cumple = false;

	const char *nombre_archivo =
		"pruebas/archivos_csv/pokemones_lineas_vacias.csv";

	tp1_t *tp_aux = tp1_leer_archivo(nombre_archivo);

	if (tp_aux != NULL) {
		cumple = tp1_cantidad(tp_aux) == CANT_POKEMONES_PRUEBA &&
			 es_el_tp_esperado(punteros_pokemones, tp_aux);
	}

	pa2m_afirmar(
		cumple,
		"El proceso de lectura ignora correctamente las líneas vacías del archivo.");

	tp1_destruir(tp_aux);
}

void prueba_ignorar_pokemones_truchos()
{
	struct pokemon pokemones[CANT_POKEMONES_PRUEBA];
	struct pokemon *punteros_pokemones[CANT_POKEMONES_PRUEBA];

	inicializar_escenario_estandar(pokemones, punteros_pokemones);

	bool cumple = false;

	const char *nombre_archivo =
		"pruebas/archivos_csv/pokemones_tramposo.csv";

	tp1_t *tp_aux = tp1_leer_archivo(nombre_archivo);

	if (tp_aux != NULL) {
		cumple = tp1_cantidad(tp_aux) == 5 &&
			 es_el_tp_esperado(punteros_pokemones, tp_aux);
	}

	pa2m_afirmar(
		cumple,
		"El proceso de lectura ignora correctamente las líneas con formato inválido o campos faltantes.");

	tp1_destruir(tp_aux);
}

void prueba_conservar_el_primero_de_repetidos()
{
	size_t cant_leida = 0;
	const char *archivo = "pruebas/archivos_csv/pokemones_repetidos.csv";
	tp1_t *tp = tp1_leer_archivo(archivo);
	struct pokemon *primero = NULL;
	bool cumple = false;

	if (tp != NULL) {
		cant_leida = tp1_cantidad(tp);
		primero = tp1_buscar_orden(tp, 0);
		cumple = (cant_leida == 1 && primero != NULL &&
			  primero->ataque == 1 && primero->defensa == 2 &&
			  primero->velocidad == 3);
	}

	pa2m_afirmar(
		cumple,
		"Al leer repetidos, solo se conserva uno, especificamente el que se lea primero.");

	tp1_destruir(tp);
}

void prueba_filtrar_tipo_cant()
{
	tp1_t *tp_filtrado = NULL;
	bool cumple = false;

	const char *nombre_archivo =
		"pruebas/archivos_csv/pokemones_varios.csv";

	tp1_t *tp_aux = tp1_leer_archivo(nombre_archivo);

	if (tp_aux != NULL) {
		tp_filtrado = tp1_filtrar_tipo(tp_aux, TIPO_FUEG);
		cumple = tp1_cantidad(tp_filtrado) == 3;
	}

	pa2m_afirmar(
		cumple,
		"Filtrar por tipo devuelve un nuevo tp1 con la cantidad exacta de pokemones esperada.");

	tp1_destruir(tp_filtrado);
	tp1_destruir(tp_aux);
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

	bool cumple = false;

	tp1_t *tp_aux = tp1_leer_archivo(nombre_archivo);
	tp1_t *tp_filtrado = NULL;

	if (tp_aux != NULL) {
		tp_filtrado = tp1_filtrar_tipo(tp_aux, TIPO_FUEG);
		cumple = (tp1_cantidad(tp_filtrado) == 3) &&
			 es_el_tp_esperado(pokemones_aux, tp_filtrado);
	}

	pa2m_afirmar(
		cumple,
		"Filtrar por tipo devuelve un nuevo tp1 con los pokemones correctos y sus atributos intactos.");

	tp1_destruir(tp_filtrado);
	tp1_destruir(tp_aux);
}

void filtrar_por_tipo_no_disponible()
{
	tp1_t *tp_filtrado = NULL;
	bool cumple = false;

	const char *nombre_archivo =
		"pruebas/archivos_csv/pokemones_varios.csv";

	tp1_t *tp_aux = tp1_leer_archivo(nombre_archivo);

	if (tp_aux != NULL) {
		tp_filtrado = tp1_filtrar_tipo(tp_aux, TIPO_FANT);
		cumple = tp_filtrado != NULL && tp1_cantidad(tp_filtrado) == 0;
	}

	pa2m_afirmar(
		cumple,
		"Filtrar un tipo inexistente en el archivo devuelve un TDA válido pero vacío.");

	tp1_destruir(tp_filtrado);
	tp1_destruir(tp_aux);
}

void prueba_encontrar_pokemon_nombre()
{
	struct pokemon pokemon_correcto_1;
	cargar_pokemon(&pokemon_correcto_1, "Vulpix", TIPO_FUEG, 41, 40, 65);

	struct pokemon *buscado = NULL;
	bool cumple = false;
	const char *nombre_archivo =
		"pruebas/archivos_csv/pokemones_varios.csv";

	tp1_t *tp_aux = tp1_leer_archivo(nombre_archivo);

	if (tp_aux != NULL)
		buscado = tp1_buscar_nombre(tp_aux, "Vulpix");

	if (buscado != NULL)
		cumple = es_el_mismo_pokemon(&pokemon_correcto_1, buscado);

	pa2m_afirmar(
		cumple,
		"Buscar un pokemon por su nombre devuelve el registro correcto.");

	tp1_destruir(tp_aux);
}

void prueba_buscar_nombre_mal_escrito()
{
	struct pokemon pokemon_correcto_1;
	cargar_pokemon(&pokemon_correcto_1, "Vulpix", TIPO_FUEG, 41, 40, 65);

	struct pokemon *buscado = NULL;
	bool cumple = false;
	const char *nombre_archivo =
		"pruebas/archivos_csv/pokemones_varios.csv";

	tp1_t *tp_aux = tp1_leer_archivo(nombre_archivo);

	if (tp_aux != NULL)
		buscado = tp1_buscar_nombre(tp_aux, "VULpix");

	if (buscado != NULL)
		cumple = es_el_mismo_pokemon(&pokemon_correcto_1, buscado);

	pa2m_afirmar(
		cumple,
		"La búsqueda por nombre ignora correctamente las diferencias entre mayúsculas y minúsculas.");

	tp1_destruir(tp_aux);
}

void prueba_buscar_pokemon_inexistente()
{
	struct pokemon *buscado = NULL;
	bool cumple = false;
	const char *nombre_archivo =
		"pruebas/archivos_csv/pokemones_varios.csv";

	tp1_t *tp_aux = tp1_leer_archivo(nombre_archivo);

	if (tp_aux != NULL)
		buscado = tp1_buscar_nombre(tp_aux, "Inexistente");

	if (tp_aux != NULL && buscado == NULL)
		cumple = true;

	pa2m_afirmar(cumple, "Buscar un nombre inexistente devuelve NULL.");

	tp1_destruir(tp_aux);
}

void prueba_buscar_n()
{
	struct pokemon *pokemon_aux = NULL;
	struct pokemon pokemon_correcto_1;
	cargar_pokemon(&pokemon_correcto_1, "Bulbasaur", TIPO_PLAN, 49, 49, 45);
	const char *archivo = "pruebas/archivos_csv/pokemones_filtrados.csv";
	bool cumple = false;

	tp1_t *tp_aux = tp1_leer_archivo(archivo);
	if (tp_aux != NULL) {
		pokemon_aux = tp1_buscar_orden(tp_aux, 0);
		cumple = es_el_mismo_pokemon(pokemon_aux, &pokemon_correcto_1);
	}

	pa2m_afirmar(
		cumple,
		"Buscar un pokemon por su índice devuelve el registro correcto según el orden alfabético.");

	tp1_destruir(tp_aux);
}

void prueba_buscar_posicion_inexistente()
{
	struct pokemon *pokemon_aux = NULL;
	const char *archivo = "pruebas/archivos_csv/pokemones_filtrados.csv";
	bool cumple = false;

	tp1_t *tp_aux = tp1_leer_archivo(archivo);

	if (tp_aux != NULL) {
		pokemon_aux = tp1_buscar_orden(tp_aux, 400);
		cumple = pokemon_aux == NULL;
	}

	pa2m_afirmar(
		cumple,
		"Buscar el pokemon en el índice N (fuera de rango) devuelve NULL.");

	tp1_destruir(tp_aux);
}

void prueba_guardar_archivo()
{
	struct pokemon pokemones[CANT_POKEMONES_PRUEBA];
	struct pokemon *punteros_pokemones[CANT_POKEMONES_PRUEBA];

	inicializar_escenario_estandar(pokemones, punteros_pokemones);

	bool cumple = false;

	const char *nombre_archivo =
		"pruebas/archivos_csv/pokemones_desordenados.csv";
	const char *archivo_escribir =
		"pruebas/archivos_csv/pokemones_escritos.csv";

	tp1_t *tp_aux = tp1_leer_archivo(nombre_archivo);
	tp1_t *tp_releido = NULL;

	if (tp_aux != NULL && es_el_tp_esperado(punteros_pokemones, tp_aux)) {
		tp1_guardar_archivo(tp_aux, archivo_escribir);
		tp_releido = tp1_leer_archivo(archivo_escribir);
		cumple = son_el_mismo_tp(tp_aux, tp_releido);
	}

	pa2m_afirmar(
		cumple,
		"Guardar el tp1 en un archivo y volver a leerlo genera una estructura idéntica a la original.");

	tp1_destruir(tp_aux);
	tp1_destruir(tp_releido);
}

void prueba_iterador_corte_anticipado()
{
	size_t visitados = 0;

	const char *archivo = "pruebas/archivos_csv/pokemones_desordenados.csv";
	tp1_t *tp = tp1_leer_archivo(archivo);

	if (tp != NULL)
		visitados = tp1_con_cada_pokemon(tp, es_rapido, NULL);

	pa2m_afirmar(
		visitados == 3,
		"El iterador interrumpe su ejecución correctamente en el momento en que la función callback devuelve false.");

	tp1_destruir(tp);
}

void prueba_iterar_sobre_todos()
{
	size_t visitados = 0;
	bool cumple = false;

	const char *archivo = "pruebas/archivos_csv/pokemones_desordenados.csv";
	tp1_t *tp = tp1_leer_archivo(archivo);

	if (tp != NULL) {
		visitados = tp1_con_cada_pokemon(tp, es_un_pokemon, NULL);
		cumple = visitados == tp1_cantidad(tp);
	}

	pa2m_afirmar(
		cumple,
		"El iterador recorre la totalidad de elementos si la función devuelve siempre true.");

	tp1_destruir(tp);
}

void pruebas_tp1_destruir()
{
	prueba_destruir_tp_vacio();
}

void pruebas_tp1_leer_archivo()
{
	prueba_tp1_leer_archivo_vacio();
	prueba_tp1_leer_algo();
	prueba_tp1_leer_cantidad_correcta();
	prueba_tp1_uno_bien();
	prueba_ignorar_pokemones_truchos();
	prueba_pokemones_desordenados();
	prueba_ignorar_lineas_vacias();
	prueba_conservar_el_primero_de_repetidos();
}

void pruebas_tp1_filtrar_tipo()
{
	prueba_filtrar_tipo_cant();
	filtrar_pokemones_correctamente();
	filtrar_por_tipo_no_disponible();
}

void pruebas_tp1_buscar_nombre()
{
	prueba_encontrar_pokemon_nombre();
	prueba_buscar_nombre_mal_escrito();
	prueba_buscar_pokemon_inexistente();
}

void pruebas_tp1_guardar_archivo()
{
	prueba_guardar_archivo();
}

void pruebas_tp1_buscar_n()
{
	prueba_buscar_n();
	prueba_buscar_posicion_inexistente();
}

void pruebas_tp1_con_cada_pokemon()
{
	prueba_iterador_corte_anticipado();
	prueba_iterar_sobre_todos();
}

void pruebas_manejando_null()
{
	pa2m_afirmar(tp1_cantidad(NULL) == 0,
		     "Obtener cantidad de un tp1 NULL devuelve 0.");
	pa2m_afirmar(tp1_buscar_nombre(NULL, "Pikachu") == NULL,
		     "Buscar por nombre en un tp1 NULL devuelve NULL.");
	pa2m_afirmar(tp1_buscar_orden(NULL, 0) == NULL,
		     "Buscar por orden en un tp1 NULL devuelve NULL.");
	pa2m_afirmar(tp1_filtrar_tipo(NULL, TIPO_FUEG) == NULL,
		     "Filtrar un tp1 NULL devuelve NULL.");

	tp1_destruir(NULL);
	pa2m_afirmar(
		true,
		"Llamar a tp1_destruir con NULL no genera errores de memoria.");
}

int main()
{
	pa2m_nuevo_grupo("============== PRUEBAS DEL TP1 ===============");

	pa2m_nuevo_grupo("Pruebas de tp1_destruir");
	pruebas_tp1_destruir();

	pa2m_nuevo_grupo("Pruebas de tp1_leer_archivo");
	pruebas_tp1_leer_archivo();

	pa2m_nuevo_grupo("Pruebas de tp1_filtrar_tipo");
	pruebas_tp1_filtrar_tipo();

	pa2m_nuevo_grupo("Pruebas de tp1_buscar_nombre");
	pruebas_tp1_buscar_nombre();

	pa2m_nuevo_grupo("Pruebas de tp1_guardar_archivo");
	pruebas_tp1_guardar_archivo();

	pa2m_nuevo_grupo("Pruebas de tp1_buscar_orden");
	pruebas_tp1_buscar_n();

	pa2m_nuevo_grupo("Pruebas de tp1_con_cada_pokemon");
	pruebas_tp1_con_cada_pokemon();

	pa2m_nuevo_grupo("Pruebas de todas las funciones al recibir NULL");
	pruebas_manejando_null();

	return pa2m_mostrar_reporte();
}