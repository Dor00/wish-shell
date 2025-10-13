#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ARGS 64

// Mensaje de error genérico :cite[5]
const char* ERROR_MESSAGE = "An error has occurred\n";

int main(int argc, char *argv[]) {
    FILE *input_stream = stdin;
    int batch_mode = 0;
    
    // Configurar modo (interactivo o por lotes)
    if (argc == 2) {
        input_stream = fopen(argv[1], "r");
        if (input_stream == NULL) {
            write(STDERR_FILENO, ERROR_MESSAGE, strlen(ERROR_MESSAGE));
            exit(1);
        }
        batch_mode = 1;
    } else if (argc > 2) {
        write(STDERR_FILENO, ERROR_MESSAGE, strlen(ERROR_MESSAGE));
        exit(1);
    }
    
    char line[MAX_LINE_LENGTH];
    
    // Bucle principal del shell
    while (1) {
        // Mostrar prompt solo en modo interactivo
        if (!batch_mode) {
            printf("wish> ");
            fflush(stdout);
        }
        
        // Leer línea de entrada
        if (fgets(line, sizeof(line), input_stream) == NULL) {
            break; // Fin de archivo
        }
        
        // Eliminar salto de línea
        line[strcspn(line, "\n")] = 0;
        
        // Saltar líneas vacías
        if (strlen(line) == 0) {
            continue;
        }
        
        // Ejecutar comando
        pid_t pid = fork();
        if (pid == 0) {
            // Proceso hijo
            char *args[MAX_ARGS];
            int arg_count = 0;
            
            // Tokenizar comando
            char *token = strtok(line, " ");
            while (token != NULL && arg_count < MAX_ARGS - 1) {
                args[arg_count++] = token;
                token = strtok(NULL, " ");
            }
            args[arg_count] = NULL;
            
            // Ejecutar comando
            execvp(args[0], args);
            
            // Si execvp falla, mostrar error
            write(STDERR_FILENO, ERROR_MESSAGE, strlen(ERROR_MESSAGE));
            exit(1);
        } else if (pid > 0) {
            // Proceso padre - esperar al hijo
            wait(NULL);
        } else {
            write(STDERR_FILENO, ERROR_MESSAGE, strlen(ERROR_MESSAGE));
        }
    }
    
    if (batch_mode) {
        fclose(input_stream);
    }
    
    return 0;
}