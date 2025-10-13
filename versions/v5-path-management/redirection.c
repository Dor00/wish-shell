#include "redirection.h"
#include <fcntl.h>
#include <unistd.h>

extern const char* ERROR_MESSAGE;

void handle_output_redirection(char **args, int *arg_count, char *output_file) {
    if (output_file == NULL) return;
    
    int fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        write(STDERR_FILENO, ERROR_MESSAGE, strlen(ERROR_MESSAGE));
        exit(1);
    }
    
    // Redirigir stdout al archivo
    if (dup2(fd, STDOUT_FILENO) < 0) {
        write(STDERR_FILENO, ERROR_MESSAGE, strlen(ERROR_MESSAGE));
        close(fd);
        exit(1);
    }
    
    // Redirigir stderr al mismo archivo
    if (dup2(fd, STDERR_FILENO) < 0) {
        write(STDERR_FILENO, ERROR_MESSAGE, strlen(ERROR_MESSAGE));
        close(fd);
        exit(1);
    }
    
    close(fd);
}

int parse_redirection(char *line, char **args, int *arg_count, char **output_file) {
    *arg_count = 0;
    *output_file = NULL;
    int found_redirect = 0;
    
    char *token = strtok(line, " \t");
    while (token != NULL && *arg_count < MAX_ARGS - 1) {
        if (strcmp(token, ">") == 0) {
            if (found_redirect) {
                write(STDERR_FILENO, ERROR_MESSAGE, strlen(ERROR_MESSAGE));
                return 0;
            }
            found_redirect = 1;
            
            token = strtok(NULL, " \t");
            if (token == NULL) {
                write(STDERR_FILENO, ERROR_MESSAGE, strlen(ERROR_MESSAGE));
                return 0;
            }
            *output_file = token;
        } else {
            args[(*arg_count)++] = token;
        }
        token = strtok(NULL, " \t");
    }
    
    args[*arg_count] = NULL;
    return 1;
}