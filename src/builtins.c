//Este archivo implementa los comandos internos del shell que se ejecutan 
//directamente en el proceso actual (sin crear procesos hijos).

// Archivo: src/builtins.c
#include "wish.h"           // Nuestro header con declaraciones
#include <stdio.h>          // Para funciones de entrada/salida
#include <stdlib.h>         // Para exit(), free()
#include <string.h>         // Para strcmp(), strlen()
#include <unistd.h>         // Para chdir(), write()

// ***Función para identificar comandos built-in: ***
int is_builtin_command(char **args, int arg_count) {
    if (arg_count == 0) return 0;     //No hay comandos, retorna falso
    
    // Compara el primer argumento con comandos built-in conocidos:
    if (strcmp(args[0], "exit") == 0) return 1;  //Comando exit
    if (strcmp(args[0], "cd") == 0) return 1;    //Comando cd
    if (strcmp(args[0], "path") == 0) return 1;  //Comando path
    
    return 0;  //No es un comando built-in
}
// ***Función principal de ejecución de comandos built-in: ***
void execute_builtin(char **args, int arg_count) {
    // --- COMANDO EXIT ---
    if (strcmp(args[0], "exit") == 0) {
        if (arg_count > 1) {  // Verifica que no tenga argumentos extra
            write(STDERR_FILENO, error_message, strlen(error_message));
            return;  // Sale sin ejecutar exit
        }
        free_path();  //Libera memoria de las rutas
        exit(0);      //Termina el programa exitosamente
    }
    
    // --- COMANDO CD (Change Directory) ---
    else if (strcmp(args[0], "cd") == 0) {
        if (arg_count != 2) {  //Debe tener exactamente 1 argumento
            write(STDERR_FILENO, error_message, strlen(error_message));
            return;
        }
        if (chdir(args[1]) != 0) {  //Intenta cambiar directorio
            //Si chdir falla (directorio no existe)
            write(STDERR_FILENO, error_message, strlen(error_message));
        }
        //Si chdir tiene éxito, no se imprime nada
    }
    
    // --- COMANDO PATH ---
    else if (strcmp(args[0], "path") == 0) {
        char *new_path[MAX_PATH_LENGTH];  // Array temporal para nuevas rutas
        int new_count = 0;                // Contador de nuevas rutas
        
        // Copia todos los argumentos (excepto el comando mismo) al nuevo array
        for (int i = 1; i < arg_count && new_count < MAX_PATH_LENGTH; i++) {
            new_path[new_count++] = args[i];  // ✅ Agrega ruta al array
        }
        
        // Llama a función que actualiza el PATH del shell
        add_to_path(new_path, new_count);
    }
}