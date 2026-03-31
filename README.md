<div align="right">
    <img width="32px" src="img/algo2.svg">
</div>

# TP


## Información del estudiante

* Lautaro Jesús Duarte Vera
* 114088
* lautarojesussss@gmail.com

---

## Índice
* [1. Instrucciones](#1-Instrucciones)
  * [1.1. Compilar el proyecto](#11-Compilar-el-proyecto)
  * [1.2. Ejecutar las pruebas](#12-Ejecutar-las-pruebas)
  * [1.3. Ejecutar el programa con Valgrind](#13-Ejecutar-el-programa-con-Valgrind)
* [2. Funcionamiento](#2-Funcionamiento)
* [3. Estructura](#3-Estructura)
  * [3.1. Diagrama de memoria](#31-Diagrama-de-memoria)
  * [3.2. Análisis de complejidades](#32-Análisis-de-complejidades)
* [4. Decisiones de diseño y/o complejidades de implementación](#4-Decisiones-de-diseño-yo-complejidades-de-implementación)
* [5. Respuestas a las preguntas teóricas](#5-Respuestas-a-las-preguntas-teóricas)

## 1. Instrucciones

### 1.1. Compilar el proyecto
```bash
make
```

### 1.2. Ejecutar las pruebas
```bash
make run
```

### 1.3. Ejecutar el programa con Valgrind
```bash
make valgrind-main
```

## 2. Funcionamiento

El tp1_t y sus primitivas funciona para guardar y consultar información de diferentes pokemones, después de cargar un tp1 con pokemones de un archivo se puede consultar sobre un pokemon dando su nombre o su posición por orden alfabético, y también consultar por varios pokemones dando el tipo que se quiere obtener; se le puede aplicar cambios a los pokemones del tp1_t usando el iterador interno tp1_con_cada_pokemon, y se puede consultar la cantidad total de pokemones en un tp1.

<div align="center">
  <img src="img/diagramas/tp1_leer_archivo.svg" width="70%">
  <p>Diagrama de flujo de tp1_leer_archivo.</p>
</div>
<div align="center">
  <img src="img/diagramas/tp1_cantidad.svg" width="70%">
  <p>Diagrama de flujo tp1_cantidad.</p>
</div>
<div align="center">
  <img src="img/diagramas/tp1_guardar_archivo.svg" width="70%">
  <p>Diagrama de flujo de tp1_guardar_archivo.</p>
</div>
<div align="center">
  <img src="img/diagramas/tp1_filtrar_tipo.svg" width="70%">
  <p>Diagrama de flujo de tp1_filtrar_tipo.</p>
</div>
<div align="center">
  <img src="img/diagramas/tp1_buscar_nombre.svg" width="70%">
  <p>Diagrama de flujo de tp1_buscar_nombre.</p>
</div>
<div align="center">
  <img src="img/diagramas/tp1_buscar_orden.svg" width="70%">
  <p>Diagrama de flujo de tp1_buscar_orden.</p>
</div>
<div align="center">
  <img src="img/diagramas/tp1_con_cada_pokemon.svg width="100%">
  <p>Diagrama de flujo de tp1_con_cada_pokemon.</p>
</div>
<div align="center">
  <img src="img/diagramas/tp1_destruir.svg" width="70%">
  <p>Diagrama de flujo de tp1_destruir.</p>
</div>


## 3. Estructura
Para el tp1_t decidí usar un vector de punteros a struct pokemon que los tenga ordenados por orden alfabético y un arreglo de arreglos de punteros que tengan cada uno solo a los punteros a pokemones de un tipo (ELEC, FUEG, NORM etc etc), y tengo los respectivos topes de todos los vectores además de una variable booleana para saber si al destruir un tp1_t debo liberar también a los pokemones o solamente los punteros que este tp1_t tenía hacia ellos.


### 3.1. Diagrama de memoria

<div align="center">
  <img src="img/diagramas/struct_tp1_t_3.svg" width="70%">
  <p>Diagrama de memoria de tp1_t.</p>
</div>


### 3.2. Análisis de complejidades

### 3.2. Análisis de complejidades (EJEMPLO 2)
|      Función      |Complejidad|                 Justificación                  |
|:-----------------:|:---------:|:----------------------------------------------:|
| `tp1_cantidad`       |  $O(1)$   |Independientemente de la cantidad de punteros que tengan los arreglos del tp1_t sacar la cantidad total es simplemente consultar el campo size_t cantidad_total y nada más, es decir, es de complejidad asintotica constante.|
|      `tp1_buscar_orden`       |  $O(1)$   |No importa qué posición tenga el pokemon solicitado en todos los casos hago un acceso directo a esa posición y devuelvo el valor, no tengo que recorrer el arreglo, así que la complejidad asintotica es constante.|
|      `tp1_destruir`       |  $O(n)$ |La complejidad es lineal porque en caso de que el tp1_t pasado sea un tp1 dueño de los datos debo recorrer el arreglo que tiene a todos los pokemones e ir liberandolos, luego a parte libero los vectores exclusivos de cada tipo pero eso es constante porque son siempre 8 operaciones.|
| `tp1_guardar_archivo`       |  $O(n)$   |Es básicamente un caso de uso para el iterador interno, que es lineal, y la función que aplico es solamente una para escribir un pokemón, por lo tanto no hay bucles ni iteraciones, en todo caso esa función de escribir_pokemon depende del largo del nombre del pokemon. Si asumo n como la cantidad de pokemones del tp1_t tp1_guardar_archivo es de complejidad asintotica lineal.|
| `tp1_buscar_nombre`       |  $O(log(n))$   |El algortimo que uso para encontrar el pokemon solicitado dentro del vector de pokemones_nombre es la busqueda binaria, es un algortimo de divide y vencerás al que se le puede aplicar el teorema maestro, la parte función tiene una sola llamada recursiva así que las llamadas no crecen a medida que se avanza en los niveles del árbol de llamadas, es decir, el factor de ramificación es 1, y la parte no recursiva es constante, así que en todos los niveles el trabajo no solo es igual sino que es de complejidad asintotica constante, y la complejidad asintotica de la función en general se puede calcular con la cantidad de niveles, y que se calculan con el log en base b de n, siendo b el factor de reducción y n el tamaño del problema.|
| `tp1_filtrar_tipo`       |  $O(n)$   |En esta función básicamente lo que hago son simples copias de un arreglo dado que siempre es menor o igual al arreglo que contiene a todos los pokemones, si asumimos que n es la cantidad total de pokemones del tp1_t entonces en el peor de los casos (que justo todos los pokemones del tp1_t sean del tipo solicitado) la complejidad es 2n o sea la complejidad asintotica es O(n).|
| `tp1_con_cada_pokemon`       |  $O(n)$   |Hago una iteración sobre el arreglo pokemones_nombre que tiene todos los pokemones, así que en el peor de los casos es justo n la cantidad de operaciones y eso se multiplica por la complejidad de f, que no conozco, por ende O(n.O(f)).|
| `tp1_leer_archivo`       |  $O(n.log(n))$   |si .|

## 4. Decisiones de diseño y/o complejidades de implementación
Decidí que el grueso del trabajo ocurra en la función tp1_leer_archivo, que tiene complejidad asintotica O(nlog(n)) ahí me encargo de leer los archivos, validar las lineas, crear y cargar los struct pokemon, ordenarlos por orden alfabético, quitar los repetidos, contar los pokemones por tipo y finalmente ordenar a los pokemones por su tipo, así puedo hacer que las funciones de consultas al tp1_t y de filtrado tengan una complejidad asintotica constante o dependiente de la cantidad de pokemones del tipo en cuestión que se pretende filtrar y no de la cantidad de todos los pokemones.

Para la carga en bruto de los punteros a los pokemones use complejidad amortizada, así evitaba que tp1_leer_archivo fuese O(n cuadarado) por los reallocs, y para el orden alfabético uso merge sort y strcasecmp; luego hago dos iteraciones distintas , una para ir quitando los repetidos y contabilizar los únicos en función de su tipo, y otra para ir acomodando copias de los punteros en los arreglos que están dedicados a un solo tipo de pokemones, podría haberlo hecho con una sola iteración esas cosas pero ví que quedaba demasiado confuso el código, y como al final agregar otra iteración no aumenta la complejidad asíntotica de la función decidí separarlo en dos iteraciones diferentes.

En la función tp1_buscar_nombre utilice busqueda binaria para hacer que la complejidad asintotica de la función no fuese lineal sino logaritmica, aprovechando que tengo el vector pokemones_nombre que los tiene ordenados alfabéticamente.

Para tp1_filtrar_tipo aprovecho que tengo los arreglados exclusivos de cada tipo y directamente me encargo de copiar la información del tipo en cuestión en un nuevo tp1, especificamente copio la info del vector exclusivo con el tipo solicitado y lo pongo tanto en el vector pokemones_nombre como en el vector exclusivo del tipo solicitado, y cambió los campos que representan los topes, el resto ya viene inicializados en cero, y en el caso de los punteros en NULL, así que los dejo como estaban. De esta manera evito que la complejidad asintotica de la función sea lineal, salvo que justo los n pokemones del tp1 sean todos del tipo solicitado.

## 5. Respuestas a las preguntas teóricas
Deberás incluir en esta sección las respuestas a las preguntas teóricas indicadas en el [enunciado](./ENUNCIADO.md) del TP.

## 5. Respuestas a las preguntas teóricas (EJEMPLO)

### 5.1. ¿Porqué...?
Respondido en su respectiva sección.

### 5.2 ¿Cómo...?
Para implementar el....

### 5.3 ¿Cuál fue el...?
El motivo fue....
