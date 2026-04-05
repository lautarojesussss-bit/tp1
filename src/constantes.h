#ifndef CONSTANTES_H_
#define CONSTANTES_H_

//tp1.c
#define TAMANIO_INICIAL 101
#define CANT_DATOS 5
#define SEPARADOR ','
#define TIPOS_CANT 8
#define CANT_METRICAS_POKEMON 3

//utils.c
#define FORMATO_ESCRITURA "%s,%s,%i,%i,%i\n"

//main.c
#define ERROR -1
#define CANT_ARGUMENTOS 4
#define ACCION_1 "mostrar"
#define ACCION_2 "buscar"
#define ACCION_1_FORMA_1 "nombre"
#define ACCION_1_FORMA_2 "tipo"
#define CANT_TIPOS 8
#define FORMATO_IMPRESION \
	"Nombre: %-15s | Tipo: %-10s | Ataque: %-3d | Defensa: %-3d | Velocidad: %-3d\n"

//pruebas_alumno.c
#define MAX_NOMBRE 200
#define ARCHIVO_UN_POKEMON "pruebas/archivos_csv/un_pokemon.csv"
#define ARCHIVO_VACIO "pruebas/archivos_csv/vacio.csv"
#define NOMBRE_ARCHIVO_NUEVO "pruebas/archivos_csv/pokemones_filtrados.csv"
#define ARCHIVO_VARIOS_POKEMONES "pruebas/archivos_csv/5pokemones.csv"
#define CANT_LEIDA_ESPERADA 5
#define N_POKEMON 2
#define POKEMON_BUSCADO "Pikachu"
#define CANT_POKEMONES_PRUEBA 5

#endif //CONSTANTES_H_