#include "parallel.h"
#include <sys/wait.h>
#include <string.h>

#define MAX_PARALLEL_COMMANDS 10

void execute_parallel_commands(char *input_line) {
    char *commands[MAX_PARALLEL_COMMANDS];
    int command_count = 0;
    pid_t child_pids[MAX_PARALLEL_COMMANDS];
    int pid_count = 0;
    
    // Dividir comandos por '&' :cite[3]
    char *command_token = strtok(input_line, "&");
    while (command_token != NULL && command_count < MAX_PARALLEL_COMMANDS) {
        // Limpiar espacios en blanco
        while (*command_token == ' ') command_token++;
        char *end = command_token + strlen(command_token) - 1;
        while (end > command_token && *end == ' ') {
            *end = '\0';
            end--;
        }
        
        if (strlen(command_token) > 0) {
            commands[command_count++] = command_token;
        }
        command_token = strtok(NULL, "&");
    }
    
    // Ejecutar comandos en paralelo
    for (int i = 0; i < command_count; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            // Proceso hijo
            execute_single_command(commands[i]);
            exit(0);
        } else if (pid > 0) {
            // Proceso padre - almacenar PID
            child_pids[pid_count++] = pid;
        } else {
            write(STDERR_FILENO, ERROR_MESSAGE, strlen(ERROR_MESSAGE));
        }
    }
    
    // Esperar a que todos los procesos hijos terminen
    for (int i = 0; i < pid_count; i++) {
        waitpid(child_pids[i], NULL, 0);
    }
}