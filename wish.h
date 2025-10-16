// Archivo: wish.h

#ifndef WISH_H
#define WISH_H

// --- Constantes ---
#define MAX_ARGS 64
#define MAX_PATH_LENGTH 1024
#define MAX_COMMAND_LENGTH 1024

// --- Variables Globales ---
extern char *search_path[MAX_PATH_LENGTH];
extern int path_count;
extern char error_message[];

// --- Prototipos de Funciones ---
void initialize_shell();
void parse_command(char *line, char **args, int *arg_count, char **output_file);
void execute_command(char **args, int arg_count, char *output_file);
int is_builtin_command(char **args, int arg_count);
void execute_builtin(char **args, int arg_count);
void find_executable_path(char *command, char *full_path);
void add_to_path(char **directories, int count);
void free_path();

#endif // WISH_H