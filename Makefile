# Archivo: Makefile

# --- Variables de Configuración ---

CC = gcc
# CFLAGS: Banderas de compilación
# -Wall y -Wextra: Habilitan todas las advertencias y advertencias extra.
# -std=c99: Especifica el estándar C99.
CFLAGS = -Wall -Wextra -std=c99 -I. -D_GNU_SOURCE
LDFLAGS = 

# Nombre del ejecutable final
TARGET = wish

# Directorios
SRC_DIR = src

# --- Archivos del Proyecto ---

# Obtiene todos los archivos .c en el directorio src/
SRCS = $(wildcard $(SRC_DIR)/*.c)

# Convierte la lista de .c a .o, manteniendo la ruta (src/file.o)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(SRC_DIR)/%.o, $(SRCS))

# Archivo de cabecera
HDRS = wish.h

# --- Reglas de Compilación ---

# .PHONY: Declara objetivos que no son archivos (para evitar conflictos si existe un archivo con ese nombre)
.PHONY: all clean

# Objetivo predeterminado: construye el ejecutable 'wish'
all: $(TARGET)

# Regla de Enlazado (Linking)
# El ejecutable 'wish' depende de todos los archivos objeto (.o)
$(TARGET): $(OBJS)
	@echo "Enlazando archivos objeto para crear el ejecutable: $@"
	$(CC) $(LDFLAGS) $(OBJS) -o $@

# Regla de Compilación (Compiling)
# Un archivo objeto (src/file.o) depende de su fuente (src/file.c) y el header (wish.h)
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c $(HDRS)
	@echo "Compilando $< a $@"
	$(CC) $(CFLAGS) -c $< -o $@

# --- Regla de Limpieza ---

# Elimina todos los archivos objeto y el ejecutable
clean:
	@echo "Limpiando archivos objeto y ejecutable..."
	rm -f $(SRC_DIR)/*.o $(TARGET)