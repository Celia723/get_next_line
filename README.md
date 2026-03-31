*Este proyecto ha sido creado como parte del currículo de 42 por cebeoyero.*

---

# 📖 get_next_line

> *Leer una línea de un descriptor de archivo es demasiado tedioso.*

---

## Descripción

`get_next_line` es una función en C que lee y devuelve una línea cada vez desde un descriptor de archivo, ya sea un fichero o la entrada estándar (`stdin`). Llamándola de forma repetida (por ejemplo dentro de un bucle), recorre el contenido del archivo línea a línea hasta que no queda nada por leer.

El objetivo de aprendizaje central del proyecto es dominar las **variables estáticas** en C: una variable que conserva su valor entre llamadas sucesivas a la función sin ser global. Esto es lo que permite a `get_next_line` "recordar" dónde se quedó entre llamadas.

### Comportamiento clave

- Devuelve la línea recién leída, incluyendo el `\n` al final (excepto al llegar al final del archivo si este no termina en `\n`).
- Devuelve `NULL` cuando no queda nada por leer o si ocurre un error.
- Funciona correctamente tanto con ficheros como con `stdin`.
- El tamaño del buffer de lectura se establece en tiempo de compilación con `-D BUFFER_SIZE=n` y afecta al uso de memoria y al rendimiento.

### Explicación y justificación del algoritmo

La función utiliza un **buffer estático por descriptor de archivo** (`static char *line[1024]`) para conservar los datos no procesados entre llamadas.

Cada llamada sigue esta lógica:

1. **Revisar el buffer estático** — si ya contiene un salto de línea (`\n`), saltar directamente al paso 4.
2. **Leer del fd** hacia un buffer temporal en el heap de `BUFFER_SIZE` bytes.
3. **Concatenar** los bytes recién leídos al buffer estático mediante `ft_strjoin`.
4. **Repetir** los pasos 2–3 hasta encontrar un `\n` o llegar al final del archivo.
5. **Separar** el buffer estático en el primer `\n`:
   - Todo hasta (e incluyendo) el `\n` se convierte en el valor de retorno.
   - Todo lo que queda después del `\n` permanece en el buffer estático para la próxima llamada.
6. Devolver la línea extraída (o `NULL` en caso de EOF / error).

Este enfoque **lee lo mínimo posible** en cada llamada — nunca lee el archivo entero de golpe.

**¿Por qué `static char *line[1024]`?** Usar un array indexado por `fd` (hasta 1024 descriptores abiertos, el límite habitual del sistema) permite gestionar múltiples descriptores de archivo simultáneamente sin mezclar sus buffers. Esto es también lo que hace funcionar la parte bonus con una sola variable estática.

---

## Archivos

| Archivo | Propósito |
|---|---|
| `get_next_line.c` | Lógica principal: `get_next_line`, bucle de lectura, separación |
| `get_next_line_utils.c` | Funciones auxiliares: `ft_strjoin`, `ft_strchr`, `ft_substr`, `ft_strlen`, `ft_strdup` |
| `get_next_line.h` | Prototipos de funciones, includes, valor por defecto de `BUFFER_SIZE` |
| `get_next_line_bonus.c` | Bonus: misma lógica (una sola variable estática + múltiples fd) |
| `get_next_line_bonus.h` | Cabecera del bonus |
| `get_next_line_utils_bonus.c` | Funciones auxiliares del bonus |

---

## Instrucciones

### Compilación

```bash
cc -Wall -Werror -Wextra -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c
```

Puedes usar cualquier entero positivo como `BUFFER_SIZE`. La función está diseñada para funcionar correctamente con valores tan pequeños como `1` o tan grandes como `10000000`, aunque valores muy grandes reservan más memoria por llamada.

### Ejemplo de uso

```c
#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

int main(void)
{
    int     fd;
    char    *line;

    fd = open("fichero.txt", O_RDONLY);
    while ((line = get_next_line(fd)) != NULL)
    {
        printf("%s", line);
        free(line);
    }
    close(fd);
    return (0);
}
```

### Restricciones

- No se permite `libft`
- No se permite `lseek`
- No se permite el uso de variables globales
- Funciones autorizadas: `read`, `malloc`, `free`

---

## Parte bonus

### ¿Qué añade el bonus?

La parte bonus exige dos mejoras sobre la parte obligatoria:

1. **Una sola variable estática** — implementar toda la lógica usando únicamente `static char *line[1024]` en lugar de varias variables estáticas.
2. **Múltiples descriptores de archivo simultáneos** — la función debe ser capaz de leer de varios `fd` distintos en la misma ejecución sin perder el estado de ninguno de ellos.

### ¿Cómo funciona la gestión de múltiples fd?

La clave está en el array `static char *line[1024]`. Cada posición del array corresponde a un descriptor de archivo diferente:

- `line[3]` guarda el buffer pendiente del fd 3
- `line[4]` guarda el buffer pendiente del fd 4
- `line[5]` guarda el buffer pendiente del fd 5
- …y así hasta 1024 descriptores simultáneos

De este modo, llamar a `get_next_line(3)`, luego a `get_next_line(4)` y volver a `get_next_line(3)` funciona correctamente: cada fd tiene su propio "puntero" de lectura guardado en su posición del array, y nunca se mezclan datos entre descriptores.

**¿Por qué exactamente 1024?** En Linux y macOS, el sistema operativo limita el número de archivos que un proceso puede tener abiertos al mismo tiempo. Este límite por defecto es **1024** (puedes comprobarlo con `ulimit -n` en tu terminal). Los descriptores de archivo son simplemente enteros no negativos asignados en orden desde 0 (`0` = stdin, `1` = stdout, `2` = stderr, y a partir del `3` los que abre el programa), por lo que el mayor fd posible en condiciones normales es `1023`. Dimensionando el array a 1024 posiciones se cubre el rango completo `[0, 1023]` sin desperdiciar memoria ni arriesgar un acceso fuera de límites.

### Ejemplo con múltiples fd

```c
#include "get_next_line_bonus.h"
#include <fcntl.h>
#include <stdio.h>

int main(void)
{
    int     fd1;
    int     fd2;
    char    *line;

    fd1 = open("archivo1.txt", O_RDONLY);
    fd2 = open("archivo2.txt", O_RDONLY);

    // Leemos alternando entre los dos archivos
    line = get_next_line(fd1); printf("fd1: %s", line); free(line);
    line = get_next_line(fd2); printf("fd2: %s", line); free(line);
    line = get_next_line(fd1); printf("fd1: %s", line); free(line);
    line = get_next_line(fd2); printf("fd2: %s", line); free(line);

    close(fd1);
    close(fd2);
    return (0);
}
```

### Compilación del bonus

```bash
cc -Wall -Werror -Wextra -D BUFFER_SIZE=42 get_next_line_bonus.c get_next_line_utils_bonus.c
```

> ⚠️ La parte bonus solo se evalúa si la parte obligatoria está completamente terminada y funciona sin ningún fallo.

---

## Recursos

### Documentación y referencias

- [`read(2)` — man page de Linux](https://man7.org/linux/man-pages/man2/read.2.html)
- [`open(2)` — man page de Linux](https://man7.org/linux/man-pages/man2/open.2.html)
- [Variables estáticas en C — GeeksforGeeks](https://www.geeksforgeeks.org/static-variables-in-c/)
- [Descriptores de archivo — manual de GNU libc](https://www.gnu.org/software/libc/manual/html_node/File-Descriptors.html)
- [Gestión de memoria en C — cppreference](https://en.cppreference.com/w/c/memory)

### Uso de IA

La IA (Claude) se utilizó exclusivamente para ayudar a redactar este archivo `README.md` — estructurando las secciones, formateando el markdown y explicando el algoritmo de forma clara. No se utilizó IA para escribir, depurar ni revisar el código fuente en C. Todas las decisiones de implementación se tomaron de forma independiente.
