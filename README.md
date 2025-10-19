
-----

# 🐚 **WISH: A Simple UNIX Shell**
		por Dorian García Giraldo
        Fredy Hoyos
        Juan Manuel restrepo

## 📝 Descripción del Proyecto

**WISH** es una implementación minimalista de una *shell* de comandos UNIX, desarrollada en el lenguaje **C**. Su propósito es emular las funcionalidades básicas de una *shell* moderna, incluyendo el manejo de comandos incorporados (*built-in*), la ejecución de programas externos mediante la ruta de búsqueda (`PATH`), la redirección de salida (`>`) y, opcionalmente, la ejecución de comandos en paralelo (`&`).

Este proyecto fue desarrollado como ejercicio de programación del laboratorio 2 de Sistemas operativos para comprender la interacción con el sistema operativo a través de las llamadas `fork()`, `execv()` y `wait()`.

## ✨ Características

| Categoría | Característica | Descripción |
| :--- | :--- | :--- |
| **Comandos** | **Ejecución Externa** | Ejecuta cualquier programa binario encontrado en los directorios definidos por la ruta (`PATH`). |
| **Built-ins** | **`exit`** | Termina la *shell*. |
| **Built-ins** | **`cd`** | Cambia el directorio de trabajo actual. |
| **Built-ins** | **`path`** | Configura la ruta de búsqueda de ejecutables. |
| **I/O** | **Redirección de Salida (`>`)** | Redirige la salida estándar (`stdout` y `stderr`) de un comando a un archivo especificado. |
| **Modos** | **Interactivo** | Lee comandos desde la entrada estándar (teclado). |
| **Modos** | **Por Lotes (*Batch*)** | Lee comandos línea por línea desde un archivo de *script* proporcionado como argumento. |

## ⚙️ Compilación y Ejecución

El proyecto utiliza un `Makefile` para automatizar el proceso de compilación, haciendo la gestión de dependencias simple y directa.

### Requisitos

  * Un compilador C (se recomienda **`gcc`**).
  * GNU Make.

### Compilación

Para compilar el código fuente y generar el ejecutable **`wish`**:

```bash
# Navega al directorio raíz del proyecto
cd /ruta/a/shell2

# Ejecuta el Makefile
make
```

Si la compilación es exitosa, se creará un ejecutable llamado **`wish`** en el directorio raíz.

### Ejecución

#### Modo Interactivo

Simplemente ejecuta el programa sin argumentos. La *shell* te presentará un *prompt* (ej. `wish>`) y esperará comandos.

```bash
./wish
```

#### Modo por Lotes (*Batch*)

Proporciona la ruta a un archivo que contenga una secuencia de comandos (un *script*) como argumento. La *shell* ejecutará todos los comandos del archivo y luego terminará.

```bash
./wish comandos.txt
```

## 📚 Comandos Disponibles

### 1\. `path [dir1 dir2 ...]`

Define los directorios donde la *shell* buscará los ejecutables.

  * Si se llama sin argumentos, **vacía la ruta de búsqueda**.
  * Si se llama con directorios, **reemplaza** la ruta actual con los directorios proporcionados.

**Ejemplos:**

```
path /bin /usr/bin     # Establece el path para buscar ls, cat, etc.
path                  # La ruta de búsqueda queda vacía (solo funcionan built-ins)
```

### 2\. `cd [directorio]`

Cambia el directorio de trabajo actual.

  * Solo acepta **un argumento**.
  * Si el directorio no existe o se proporcionan más de un argumento, imprime **"An error has occurred"**.

### 3\. `exit`

Termina la ejecución de la *shell*.

  * Solo puede ser llamado sin argumentos.
  * Si se llama con argumentos, imprime **"An error has occurred"**.

### 4\. Redirección de Salida

La redirección se realiza mediante el símbolo `>`.

**Sintaxis:** `comando [arg1 arg2 ...] > archivo_salida`

  * La *shell* solo permite un único operador de redirección (`>`) al final de la línea.
  * Si el archivo de salida ya existe, **su contenido será sobrescrito**.
  * Tanto `stdout` como `stderr` se redirigen al archivo.

**Ejemplo:**

```
echo Hola mundo > log.txt
```

## 🧹 Limpieza del Proyecto

Para eliminar todos los archivos objeto (`.o`) generados durante la compilación y el ejecutable final (`wish`), utiliza la regla `clean` del `Makefile`:

```bash
make clean
```

-----

## 🐛 Manejo de Errores

En línea con los requisitos de una *shell* simple, cualquier error encontrado (comando no encontrado, argumento incorrecto, sintaxis inválida, fallo de `fork`, etc.) resulta en el mensaje estandarizado:

```
An error has occurred
```

La *shell* **debe** continuar ejecutando los comandos siguientes (en modo interactivo o por lotes) después de imprimir un error, a menos que el error sea fatal (ej. fallo de `malloc` o problemas irrecuperables del sistema).

## ⚙️ Pruebas

Las pruebas de los comandos asociados se realiza a través del archivo comandos.txt, el cual contiene los comandos que ejecuta la shell wish, así como tambien comandos paralelos y de redirección. Para ejecutarlo escribe lo siguiente (debes estar dentro de la carpeta shell2 y por fuera de wish):

./wish comandos.txt

La salida correspondiente será parecida a la siguiente:

Makefile  Makefile:Zone.Identifier  README.md  batch.txt:Zone.Identifier  comandos.txt  src  tests  wish  wish.h
/home/dor/shell2/tests
total 8
drwxr-xr-x 2 dor dor 4096 Oct 13 21:29 .
drwxr-xr-x 4 dor dor 4096 Oct 13 21:41 ..
-rw-r--r-- 1 dor dor    0 Oct 16 10:01 log.txt
Thu Oct 16 10:01:18 -05 2025
dor