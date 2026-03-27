#include "src/tp1.h"
#include "src/utils.h"
#include "stdio.h"


#define ERROR -1
#define CANT_ARGUMENTOS 4
#define ACCION_1 "mostrar"
#define ACCION_2 "buscar"
#define ACCION_1_FORMA_1 "nombre"
#define ACCION_1_FORMA_2 "tipo"
#define CANT_TIPOS 8


void imprimir_tp_por_tipo(tp1_t *tp)
{
        bool error_memoria = false;
        tp1_t *tp_aux = NULL;

        for (size_t i = 0; !error_memoria && i < CANT_TIPOS ; i++) {
                tp_aux = tp1_filtrar_tipo(tp, i);

                if (!tp_aux) {
                        error_memoria = true;
                } else {
                        escribir_pokemones(tp_aux, stdout);
                        tp1_destruir(tp_aux);
                }
        }
}


int main(int argc, char const *argv[])
{
        if (argc != CANT_ARGUMENTOS){
                printf("La cantidad de argumentos no es correcta.\n");
                return ERROR;
        } 
        
        bool es_accion_1 = strcmp(ACCION_1, argv[2]) == 0;
        bool es_accion_2 = strcmp(ACCION_2, argv[2]) == 0;

        if (!es_accion_1 && !es_accion_2) {
                printf("La accion '%s' no está definida.\n", argv[2]);
                return ERROR;
        }

        tp1_t *tp = tp1_leer_archivo(argv[1]);

        if (!tp) { 
                printf("Error al leer el archivo.");
                return ERROR;
        }

        if (es_accion_1) {

                bool es_forma_1 = strcmp(argv[3], ACCION_1_FORMA_1) == 0;
                bool es_forma_2 = strcmp(argv[3],ACCION_1_FORMA_2) == 0;
                bool error_memoria = false;

                if (es_forma_1)
                        escribir_pokemones(tp, stdout);
                else if(es_forma_2)
                        imprimir_tp_por_tipo(tp);

                if (error_memoria)
                        printf("Fallo al efectuar la operación por problemas con la memoria.");
        } else{
                int n = -1;
                struct pokemon *pokemon_buscado = NULL;

                if (es_numero_valido(argv[3], &n)) {
                        pokemon_buscado = tp1_buscar_orden(tp, n);
                        if (!pokemon_buscado)
                                printf("Error, el pokemon %d no se encontró.\n", n);
                        else 
                                escribir_pokemon(pokemon_buscado, stdout);
                } else {
                        pokemon_buscado =  tp1_buscar_nombre(tp, argv[3]);
                        if (!pokemon_buscado)
                                printf("Error el pokemon %s no se encontró.\n", argv[3]);
                        else 
                                escribir_pokemon(pokemon_buscado, stdout);
                }
        }

        tp1_destruir(tp);

	return 0;
}
