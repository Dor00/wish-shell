// Archivo: src/wish.c

#include "wish.h"           // Nuestro header personalizado
#include <stdio.h>          // Para getline, printf, fflush
#include <stdlib.h>         // Para exit, malloc, free
#include <string.h>         // Para strdup, strtok_r, strlen, strcmp
#include <unistd.h>         // Para fork, exec, write
#include <sys/wait.h>       // Para wait, waitpid
#include <sys/types.h>      // Para pid_t


// Variables Globales
char *search_path[MAX_PATH_LENGTH] = {NULL}; // <--- Inicializar a NULL, es una array de rutas de búsqueda
int path_count = 0;   // Contador de rutas
char error_message[] = "An error has occurred\n"; // mensaje de error estandar 

// Función de inicialización
void initialize_shell() {
    search_path[0] = strdup("/bin"); // el primer directorio en el arreglo search_path sera /bin, es el directorio por defectos para ejecutar comandos
    path_count = 1; // ruta inicial
}
//***función main - configuración inicial ***
int main(int argc, char *argv[]) {
    FILE *input_stream;      // Flujo de entrada (archivo o teclado)
    int batch_mode = 0;      // Modo: 0=interactivo, 1=por lotes
    
    initialize_shell(); // Inicializa variables del shell
    
    if (argc == 1) {
        input_stream = stdin;    //un argumento: entrada interactiva
    } else if (argc == 2) {
        input_stream = fopen(argv[1], "r"); //dos argumentos: abrir archivo
        if (input_stream == NULL) {
            write(STDERR_FILENO, error_message, strlen(error_message));
            exit(1); //error al abrir archivo 
        }
        batch_mode = 1; //activamos modo por lotes
    } else {
        write(STDERR_FILENO, error_message, strlen(error_message));
        exit(1); // error, mas de 2 argumentos 
    }
    //***Variables para lectura de líneas:  ***
    char *line = NULL;       // Buffer para la línea leída
    size_t len = 0;          // Longitud del buffer
    ssize_t read;            // Bytes realmente leídos
    
    while (1) {  // Bucle principal del shell
        // Mostrar prompt solo en modo interactivo
		if (!batch_mode) {
            printf("wish> "); // imprime el promt del shell
            fflush(stdout); //fuerza a que todo lo que esté pendiente en ese flujo de salida estándar(stdout) se escriba de inmediato.
        }
        //***Lectura y procesamiento de línea:  ***
        read = getline(&line, &len, input_stream); //leer una línea completa desde un flujo de entrada (como un archivo o la entrada estándar)
        if (read == -1) {  // Error o fin de archivo
            if (feof(input_stream)) { //Fin de archivo
                free(line);
                exit(0); // salida limpia
            } else {
                write(STDERR_FILENO, error_message, strlen(error_message));
                continue; //Error de lectura, continuar
            }
        }
        // Limpiar salto de línea al final
        if (read > 0 && line[read - 1] == '\n') {
            line[read - 1] = '\0'; // Reemplazar '\n' con terminador
        }
        
        if (strlen(line) == 0) {       // Línea vacía
            continue;                   // Saltar a siguiente iteración
        }
        //***Procesamiento de comandos paralelos (con &):***
         char *command_token;           // Token de comando individual
        char *saveptr;                 // Para strtok_r (thread-safe)
        char *command_line = line;     // Línea a tokenizar
        
         // Dividir línea por '&' para comandos paralelos
		while ((command_token = strtok_r(command_line, "&", &saveptr)) != NULL) {
            command_line = NULL; // Para siguientes llamadas a strtok_r
            
            // Eliminar espacios al inicio
			while (*command_token == ' ' || *command_token == '\t') {
                command_token++;
            }
            // Eliminar espacios al final
			char *end = command_token + strlen(command_token) - 1;
            while (end > command_token && (*end == ' ' || *end == '\t')) {
                *end = '\0';  // Truncar espacios
                end--;
            }
            
            if (strlen(command_token) == 0) { // Token vacío después de limpiar
                continue;
            }
            //***Ejecución de comandos: ***
            char *args[MAX_ARGS];
            int arg_count = 0;
            char *output_file = NULL;
            
             // Parsear comando en argumentos
            parse_command(command_token, args, &arg_count, &output_file);
            
            if (arg_count > 0) { // Si hay comandos válidos
                if (is_builtin_command(args, arg_count)) {
                    execute_builtin(args, arg_count); // Comando interno
                } else {
                    // Comando externo - crear proceso hijo
					pid_t pid = fork();
                    if (pid == 0) { // Duplicar proceso
                        // Proceso HIJO
						execute_command(args, arg_count, output_file);
                        exit(0); // Salir después de ejecutar
                    } else if (pid < 0) {
						//Error en fork
                        write(STDERR_FILENO, error_message, strlen(error_message));
                    
					} else {
                        // Proceso padre: Esperar solo a este hijo si no es paralelo.
                        // SI ESTÁS TRABAJANDO CON PARALELISMO, NECESITAS UN ARRAY DE PIDS.
                        // Si no lo estás, haz un simple wait:
                        waitpid(pid, NULL, 0); // <-- Usa waitpid para esperar específicamente a que este hijo termine
                    }				
					
                }
            }
        }
        // Esperar a todos los procesos en background
        while (wait(NULL) > 0); // Recolectar procesos zombies
    }
    // ***Limpieza final: ***
    free(line); // Liberar memoria de getline
    if (batch_mode) {
        fclose(input_stream);  // Cerrar archivo si estaba abierto
    }
    free_path();  // Liberar rutas de búsqueda
    return 0; // Salida exitosa
}