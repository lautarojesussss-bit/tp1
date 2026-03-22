CC = gcc
CFLAGS = -g -std=c99 -Wall -Wconversion -Wtype-limits -Werror -I.
VFLAGS = --leak-check=full --track-origins=yes --show-reachable=yes
EJEC = pruebas
SRCS = tp1.c prueba.c split.c pokemon.c ordenamiento.c
HDRS = tp1.h split.h pokemon.h ordenamiento.h

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
	clang-format -style=file -i *.c

.PHONY: all run valgrind clean format

clean:
	rm -f $(EJEC)
