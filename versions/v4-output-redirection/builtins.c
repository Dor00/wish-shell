#include "builtins.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

// Variable global para mensaje de error
extern const char* ERROR_MESSAGE;

int is_builtin_command(char **args, int arg_count) {
    if (arg_count == 0) return 0;
    
    // Verificar si es comando incorporado
    if (strcmp(args[0], "exit") == 0) return 1;
    if (strcmp(args[0], "cd") == 0) return 1;
    if (strcmp(args[0], "path") == 0) return 1;
    
    return 0;
}

void execute_builtin(char **args, int arg_count) {
    if (strcmp(args[0], "exit") == 0) {
        // Comando exit: terminar shell
        if (arg_count > 1) {
            write(STDERR_FILENO, ERROR_MESSAGE, strlen(ERROR_MESSAGE));
            return;
        }
        exit(0);
    }
    else if (strcmp(args[0], "cd") == 0) {
        // Comando cd: cambiar directorio
        if (arg_count != 2) {
            write(STDERR_FILENO, ERROR_MESSAGE, strlen(ERROR_MESSAGE));
            return;
        }
        if (chdir(args[1]) != 0) {
            write(STDERR_FILENO, ERROR_MESSAGE, strlen(ERROR_MESSAGE));
        }
    }
    else if (strcmp(args[0], "path") == 0) {
        // Comando path: gestión básica de PATH
        // (Implementación completa en versión 5)
        printf("Comando path - disponible en versión 5\n");
    }
}