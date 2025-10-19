// Este archivo proporciona las utilidades fundamentales para gestionar el PATH 
//del shell y parsear comandos, incluyendo el manejo de 
//redirección de salida y validación de sintaxis.

// Archivo: src/utils.c
#include "wish.h"           // Nuestro header con declaraciones
#include <stdio.h>          // Para funciones de entrada/salida  
#include <stdlib.h>         // Para malloc(), free()
#include <string.h>         // Para strdup(), strtok_r(), strcmp()
#include <unistd.h>         // Para write()

// ***Función para liberar memoria del PATH: ***
void free_path() {
    // Itera sobre todas las rutas almacenadas
    for (int i = 0; i < path_count; i++) {
        free(search_path[i]);  //Libera cada string duplicado
    }
    path_count = 0;  //Reinicia el contador
}
// ***Función para actualizar el PATH del shell: ***
void add_to_path(char **directories, int count) {
    free_path();  //Primero libera las rutas antiguas
    
    // Copia las nuevas rutas al search_path
    for (int i = 0; i < count && i < MAX_PATH_LENGTH; i++) {
        search_path[i] = strdup(directories[i]);  //Duplica cada string
    }
    path_count = count;  //Actualiza el contador
}
// ***Función principal de parseo de comandos: ***
void parse_command(char *line, char **args, int *arg_count, char **output_file) {
    *arg_count = 0;        //Inicializa contador de argumentos
    *output_file = NULL;   //Inicializa archivo de salida
    
    char *token;           // Token actual
    char *saveptr;         // Para strtok_r (thread-safe)
    int found_redirect = 0; // Bandera para detectar ">"
    
    // Primer token: divide por espacios y tabs
    token = strtok_r(line, " \t", &saveptr);
	
	// ***Bucle principal de parseo: ***
     while (token != NULL) {
        // --- DETECCIÓN DE REDIRECCIÓN ---
        if (strcmp(token, ">") == 0) {
            if (found_redirect) {  //Ya se encontró un ">" anterior
                write(STDERR_FILENO, error_message, strlen(error_message));
                *arg_count = 0;  //Marca comando como inválido
                return;
            }
            found_redirect = 1;  //Marca que encontramos redirección
            
            // Obtiene el siguiente token (debería ser el nombre del archivo)
            token = strtok_r(NULL, " \t", &saveptr);
            if (token == NULL) {  //No hay archivo después de ">"
                write(STDERR_FILENO, error_message, strlen(error_message));
                *arg_count = 0;
                return;
            }
            *output_file = token;  //Guarda el nombre del archivo de salida
        } 
        // --- ARGUMENTO NORMAL ---
        else {
            if (*arg_count < MAX_ARGS - 1) {  //Verifica límite
                args[(*arg_count)++] = token;  //Agrega argumento al array
            }
        }
        // Siguiente token
        token = strtok_r(NULL, " \t", &saveptr);
    }
    //***Finalización y validaciones: ***
        // Termina el array de argumentos con NULL (requerido por execv)
    if (*arg_count < MAX_ARGS) {
        args[*arg_count] = NULL;
    }
    
    // Validación final: redirección sin comandos
    if (found_redirect && *arg_count == 0) {
        write(STDERR_FILENO, error_message, strlen(error_message));
        *arg_count = 0;  //Marca comando como inválido
    }
}