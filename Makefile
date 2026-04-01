CC = gcc
CFLAGS_BASE = -g -std=c99 -Wall -Wconversion -Wtype-limits -Werror -Isrc
CFLAGS_TEST = $(CFLAGS_BASE) -Ipruebas
CFLAGS_MAIN = $(CFLAGS_BASE)
VFLAGS = --leak-check=full --track-origins=yes --show-reachable=yes
EJEC = pruebas_alumno
EJEC_MAIN = tp1
SRCS_MAIN = main.c src/tp1.c src/utils.c src/split.c
HDRS_MAIN = src/tp1.h src/utils.h src/split.h 
SRCS = src/tp1.c src/utils.c src/split.c  pruebas/pruebas_alumno.c
HDRS = src/tp1.h src/utils.h src/split.h pruebas/pa2m.h

# 1.1. Compilar: 'make' o 'make all' solo construye el ejecutable
all: $(EJEC) $(EJEC_MAIN)

$(EJEC): $(SRCS) $(HDRS)
	$(CC) $(CFLAGS_TEST) $(SRCS) -o $(EJEC)

$(EJEC_MAIN): $(SRCS_MAIN) $(HDRS_MAIN)
	$(CC) $(CFLAGS_MAIN) $(SRCS_MAIN) -o $(EJEC_MAIN)

# 1.2. Ejecutar pruebas: solo corre el binario
run: $(EJEC)
	./$(EJEC)

run-main: $(EJEC_MAIN)
	./$(EJEC_MAIN)

# 1.3. Ejecutar con Valgrind
valgrind: $(EJEC)
	valgrind $(VFLAGS) ./$(EJEC)

valgrind-main: $(EJEC_MAIN)
	valgrind $(VFLAGS) ./$(EJEC_MAIN) 5pokemones.csv buscar PIKACHU

format:
	clang-format -style=file -i main.c src/*.c pruebas/*.c

.PHONY: all run valgrind clean format

clean:
	rm -f $(EJEC) $(EJEC_MAIN)
