CC = gcc
CFLAGS = -g -std=c99 -Wall -Wconversion -Wtype-limits -Werror -Isrc -Ipruebas
VFLAGS = --leak-check=full --track-origins=yes --show-reachable=yes
EJEC = pruebas_caja_blanca
SRCS = src/tp1.c src/utils.c src/split.c  pruebas/pruebas_caja_blanca.c 
HDRS = src/tp1.h src/utils.h src/struct_tp1.h src/split.h pruebas/pa2m.h

# 1.1. Compilar: 'make' o 'make all' solo construye el ejecutable
all: $(EJEC)

$(EJEC): $(SRCS) $(HDRS)
	$(CC) $(CFLAGS) $(SRCS) -o $(EJEC)

# 1.2. Ejecutar pruebas: solo corre el binario
run: $(EJEC)
	./$(EJEC)

# 1.3. Ejecutar con Valgrind
valgrind: $(EJEC)
	valgrind $(VFLAGS) ./$(EJEC)

format:
	clang-format -style=file -i src/*.c pruebas/*.c

.PHONY: all run valgrind clean format

clean:
	rm -f $(EJEC)
