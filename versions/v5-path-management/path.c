#include "path.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_PATH_LENGTH 1024
#define MAX_PATH_DIRS 64

char *search_path[MAX_PATH_DIRS];
int path_count = 0;
extern const char* ERROR_MESSAGE;

void initialize_default_path() {
    // Inicializar con PATH por defecto
    search_path[0] = strdup("/bin");
    search_path[1] = strdup("/usr/bin");
    path_count = 2;
}

void update_search_path(char **directories, int count) {
    // Liberar PATH anterior
    for (int i = 0; i < path_count; i++) {
        free(search_path[i]);
    }
    path_count = 0;
    
    // Copiar nuevos directorios
    for (int i = 0; i < count && i < MAX_PATH_DIRS; i++) {
        search_path[path_count++] = strdup(directories[i]);
    }
}

int find_executable_path(char *command, char *full_path) {
    // Si el comando tiene '/', usar directamente
    if (strchr(command, '/') != NULL) {
        if (access(command, X_OK) == 0) {
            strcpy(full_path, command);
            return 1;
        }
        return 0;
    }
    
    // Buscar en cada directorio del PATH
    for (int i = 0; i < path_count; i++) {
        snprintf(full_path, MAX_PATH_LENGTH, "%s/%s", search_path[i], command);
        if (access(full_path, X_OK) == 0) {
            return 1;
        }
    }
    
    return 0;
}