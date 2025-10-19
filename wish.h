// Archivo: wish.h
// en este archivo se declaran las constantes así como tambien la variables globales y el
//prototipo de las funciones de la aplicación.

//directivas del preprocesador que evitan la inclusión múltiple del mismo archivo
#ifndef WISH_H        // Si WISH_H NO está definido...
#define WISH_H        // Entonces: define WISH_H y procesa el contenido

// --- Constantes ---
#define MAX_ARGS 64           // Máximo número de argumentos por comando
#define MAX_PATH_LENGTH 1024  // Longitud máxima para rutas del sistema
#define MAX_COMMAND_LENGTH 1024 // Longitud máxima para comandos

// --- Variables Globales ---
extern char *search_path[MAX_PATH_LENGTH];  // Array de rutas para buscar ejecutables
extern int path_count;                      // Contador de rutas en search_path
extern char error_message[];                // Buffer para mensajes de error

// --- Prototipos de Funciones ---
void initialize_shell();  // Inicializa el shell y variables globales

void parse_command(char *line, char **args, int *arg_count, char **output_file);
// Parsea una línea de comando en argumentos y detecta redirección de salida

void execute_command(char **args, int arg_count, char *output_file);
// Ejecuta comandos externos (no built-in)

int is_builtin_command(char **args, int arg_count);
// Verifica si el comando es built-in (interno del shell)

void execute_builtin(char **args, int arg_count);
// Ejecuta comandos built-in (cd, exit, etc.)

void find_executable_path(char *command, char *full_path);
// Busca la ruta completa de un ejecutable en PATH

void add_to_path(char **directories, int count);
// Añade directorios al PATH del shell

void free_path();  // Libera memoria usada por las rutas de búsqueda

#endif // WISH_H  // Fin de la guarda de inclusión

//Este archivo define la interfaz pública de un shell wish, con constantes para límites, variables globales para el estado del shell, y prototipos para todas las funciones principales de procesamiento y ejecución de comandos.

