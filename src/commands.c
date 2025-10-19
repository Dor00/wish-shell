//Este archivo se encarga de encontrar y ejecutar comandos externos del sistema, 
//manejando redirección de salida y verificando la existencia de los ejecutables 
//en las rutas configuradas.


// Archivo: src/commands.c
#include "wish.h"           // Nuestro header con declaraciones
#include <stdio.h>          // Para snprintf
#include <stdlib.h>         // Para exit()
#include <string.h>         // Para strlen()
#include <unistd.h>         // Para execv, access, dup2, close
#include <fcntl.h>          // Para open(), O_WRONLY, O_CREAT, etc.
#include <sys/wait.h>       // Para wait()

//***Función para buscar ejecutables en el PATH: ***

void find_executable_path(char *command, char *full_path) {
    full_path[0] = '\0';  //Inicializa el string como vacío
    
    if (path_count == 0) {  //Si no hay rutas configuradas
        return;             // Retorna sin buscar
    }
    
    // Itera sobre todas las rutas en search_path
    for (int i = 0; i < path_count; i++) {
        // Verificación de seguridad: saltar rutas nulas
        if (search_path[i] == NULL) {
            continue;  //Evita segmentation fault
        }
        
        // Construye la ruta completa: /ruta/comando
        snprintf(full_path, MAX_PATH_LENGTH, "%s/%s", search_path[i], command);
        
        // Verifica si el archivo existe y es ejecutable
        if (access(full_path, X_OK) == 0) {
            return;  //Encontró el ejecutable, retorna con full_path lleno
        }
    }
    
    //Si llegó aquí, no encontró el comando en ninguna ruta
    full_path[0] = '\0';  // Limpia el resultado
}
// ***Función principal de ejecución de comandos externos: ***
void execute_command(char **args, int arg_count, char *output_file) {
    if (arg_count == 0) { // verifica que haya comandos
        return;
    }
    // ***Manejo de redirección de salida: ***
        if (output_file != NULL) {  //Si hay redirección de salida
        // Abre/Crea el archivo con permisos de escritura
        int fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd < 0) {  //Error al abrir archivo
            write(STDERR_FILENO, error_message, strlen(error_message));
            exit(1);
        }
        
        // Redirige stdout y stderr al archivo
        if (dup2(fd, STDOUT_FILENO) < 0 || dup2(fd, STDERR_FILENO) < 0) {
            write(STDERR_FILENO, error_message, strlen(error_message));
            exit(1);
        }
        close(fd);  //Cierra el descriptor original (ya está duplicado)
    }
    
    char full_path[MAX_PATH_LENGTH];  // Buffer para la ruta completa
    find_executable_path(args[0], full_path);  // Busca el ejecutable
    
    if (strlen(full_path) == 0) {  //No se encontró el comando
        write(STDERR_FILENO, error_message, strlen(error_message));
        exit(1);
    }
    
    // Ejecuta el programa encontrado
    if (execv(full_path, args) == -1) {  //Si execv falla
        write(STDERR_FILENO, error_message, strlen(error_message));
        exit(1);
    }
    //NOTA: execv exitoso NO RETORNA - reemplaza el proceso actual
}