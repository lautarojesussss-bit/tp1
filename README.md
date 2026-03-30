<div align="right">
    <img width="32px" src="img/algo2.svg">
</div>

# TP


## Información del estudiante

* Lautaro Jesús Duarte Vera
* 114088
* lautarojesussss@gmai.com

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

> [!TIP]
> Se recomienda usar un Makefile y colocar en esta sección los comandos Make.

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
Explicar **qué** hace el TP implementado, aclarando todas las decisiones de funcionamiento que no estaban definidas por el enunciado.

> [!IMPORTANT]
> Es muy importante entender la *diferencia entre qué y cómo*. En esta sección **NO** se busca una explicación de cómo implementaste el programa, qué funciones usaste, en qué línea, etc.; se busca una explicación de **qué** es lo que hace el programa en líneas generales.

El tp1_t y sus primitivas funciona para guardar y consultar información de diferentes pokemones, para eso decidí usar un vector de punteros a struct pokemon que los tenga ordenados por orden alfabético y un arreglo de arreglos de punteros que tengan cada uno solo a los de un tipo (ELEC, FUEG, NORM etc etc) y tengo los respectivos topes de todos los vectores y una variable booleana para saber si al destruir un tp1_t debo liberar también a los pokemones o solamente los punteros que este tp1_t tenía a ellos.

El grueso del trabajo ocurre en la función tp1_leer_archivo, ahí me encargo de leer los archivos, validar las lineas, crear y cargar los struct pokemon, ordenarlos por orden alfabético, quitar los repetidos, contar los pokemones por tipo y finalmente ordenar a los pokemones por su tipo.
Para la carga en bruto de los punteros a los pokemones use complejidad amortizada, así evitaba que tp1_leer_archivo fuese O(n cuadarado) por los reallocs, y para el orden alfabético uso merge sort y strcasecmp; hago dos pasadas distintas luego, una para ir quitando los repetidos y contabilizar los únicos en función de su tipo, y otra para ir acomodando copias de los punteros en los arreglos que están dedicados a un solo tipo de pokemones, podría haberlo hecho con una sola pasada esas cosas pero ví que quedaba demasiado confuso el código, y como al final agregar otra pasada no aumenta la complejidad asíntotica de la función decidí separarlo en dos iteraciones diferentes.

En la función tp1_buscar_nombre utilice busqueda binaria para hacer que la complejidad asintotica de la función no fuese lineal sino logaritmica, aprovechando que tengo el vector pokemones_nombre que los tiene ordenados alfabéticamente.

Para tp1_filtrar_tipo aprovecho que tengo los arreglados exclusivos de cada tipo y directamente me encargo de copiar la información del tipo en cuestión en un nuevo tp1, especificamente copio la info del vector exclusivo con el tipo solicitado y lo pongo tanto en el vector pokemones_nombre como en el vector exclusivo del tipo solicitado, y cambió los campos que representan los topes, el resto ya viene inicializados en cero, y en el caso de los punteros en NULL, así que los dejo como estaban. De esta manera evito que la complejidad asintotica de la función sea lineal, salvo que justo los n pokemones del tp1 sean todos del tipo solicitado.




## 2. Funcionamiento (EJEMPLO)
El programa recibe 7 números del usuario y una vez obtenidos todos los muestra en pantalla. Para esto define un vector estático de 7 elementos y llena el mismo con los datos que inserta el usuario; cuando termina de insertar todos los números procede a imprimirlos en pantalla.
<div align="center">
  <img src="img/diagrama_flujo_programa.svg" width="70%">
  <p>Diagrama de flujo del programa explicado con más detalle.</p>
</div>

Cuando el vector llega a su máximo el programa procede a escalarlo con un factor de crecimiento..., es decir, si el vector tenía....

## 3. Estructura
Explicar cómo se implementó la/s estructura/s pedida/s en el [enunciado](./ENUNCIADO.md). En esta sección el objetivo es explicar en líneas generales, no técnicas, qué contiene la estructura, para qué y por qué.


### 3.1. Diagrama de memoria
Realizar un diagrama de memoria de la estructura de memoria durante la ejecución del programa, esto debe incluir el stack y el heap con las estructuras contenidas en ellos.

<div align="center">
  <img src="img/diagramas/struct_tp1_t_3.svg" width="70%">
  <p>Diagrama de memoria de la estructura.</p>
</div>


### 3.2. Análisis de complejidades
Explicar las complejidades de las diversas funciones que se implementaron en el programa. Esto debe incluir al menos a las funciones de la interfaz (el .h) del programa. Además, se debe ofrecer una justificación de la complejidad, es decir, por qué es esa la complejidad Big-O y no otra.

### 3.2. Análisis de complejidades (EJEMPLO 1)
En el programa tenemos funciones auxiliares y funciones principales (las que van en el .h). Respecto a estas funciones podemos analizar que:
* `fun1` tiene una complejidad de $O(1)$ ya que tiene como parámetro... y, al leer una línea....
* `fun2` tiene una complejidad de $O(n)$ ya que tiene como parámetro..., la cual....
* `fun3` tiene una complejidad de $O(n^2)$ ya que tiene como parámetro... y se encarga de....

### 3.2. Análisis de complejidades (EJEMPLO 2)
|      Función      |Complejidad|                 Justificación                  |
|:-----------------:|:---------:|:----------------------------------------------:|
|      `fun1`       |  $O(1)$   |Tiene como parámetro... y, al leer una línea....|
|      `fun2`       |  $O(n)$   |Tiene como parámetro..., la cual....            |
|      `fun3`       |  $O(n^2)$ |Tiene como parámetro... y se encarga de....     |

## 4. Decisiones de diseño y/o complejidades de implementación
Explicar las decisiones de diseño y/o las complejidades de implementación que hubo durante la resolución del TP.

## 4. Decisiones de diseño y/o complejidades de implementación (EJEMPLO)
La mayor complejidad en el TP se encuentra en la función `foo` que requiere hacer...; es por esto que decidí.... Además, decidí que el programa haga... para mejorar la implementación.

## 5. Respuestas a las preguntas teóricas
Deberás incluir en esta sección las respuestas a las preguntas teóricas indicadas en el [enunciado](./ENUNCIADO.md) del TP.

## 5. Respuestas a las preguntas teóricas (EJEMPLO)

### 5.1. ¿Porqué...?
Respondido en su respectiva sección.

### 5.2 ¿Cómo...?
Para implementar el....

### 5.3 ¿Cuál fue el...?
El motivo fue....
