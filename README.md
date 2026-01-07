# Computacion_Grafica_Clion

## Descripción

Proyecto de prácticas de Computación Gráfica para CLion/Windows. Contiene ejemplos y ejercicios con OpenGL usando GLFW, GLAD, `stb_image` y `glm`. Incluye varios ejercicios en la carpeta `practicas` (triángulos, texturas, shaders, etc.).

## Enlace del repositorio

-   Remote: [https://github.com/Francis1918/Computacion_Grafica_Clion.git](https://github.com/Francis1918/Computacion_Grafica_Clion.git)
-   Rama activa: `master`

## Estructura del proyecto

-   `CMakeLists.txt` — Archivo principal de CMake.
-   `main.cpp` — Punto de entrada (si aplica).
-   `utils.cpp` — Utilidades varias.
-   `practicas/` — Ejercicios y ejemplos (cada archivo `.cpp` es una práctica).
    -   `B2T1.cpp` — Práctica B2T1: manejo de texturas, movimiento y control por teclado.
    -   `shaders/` — Shaders usados por las prácticas (vertex/fragment).
    -   `textures/` — Imágenes usadas por las prácticas.
-   `external/` — Dependencias integradas (glad, glfw, glm, stb, learnopengl).
-   `cmake-build-debug/` — Carpeta de salida de compilación de CLion (binarios, cache).

## Dependencias

-   CMake (>= 3.x)
-   Compilador C++ compatible (MSVC, MinGW o clang)
-   OpenGL 3.3 Core (o superior)
-   GLFW (incluido en `external/glfw`)
-   GLAD (incluido en `external/glad`)
-   `stb_image` (incluido en `learnopengl`/`stb_image.h`)
-   GLM (incluido en `external/glm`)
-   CLion (opcional) en Windows

## Compilar y ejecutar

En CLion:

-   Abrir el proyecto (directorio raíz).
-   Asegurar que CMake configure correctamente (toolchain para MSVC o MinGW).
-   Construir y ejecutar la configuración (asegurar `working directory` en la configuración de ejecución está en el root del proyecto si las rutas relativas a `practicas/` fallan).

Desde línea de comandos (ejemplo Windows / Visual Studio):

```bash
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022"
cmake --build . --config Debug
# Ejecutable generado en el binario del proyecto (o en cmake-build-debug)
```

Ejecutable directo (CLion / Ninja):

-   Ejecutable de ejemplo: `cmake-build-debug/Computacion_Grafica_Clion.exe`

## Notas de rutas y recursos

-   Muchos shaders y texturas se cargan con rutas relativas como `../practicas/shaders/...` o `../practicas/textures/...`. Si la aplicación no encuentra archivos, ajustar el *working directory* de la configuración de ejecución para que las rutas relativas apunten al directorio del proyecto raíz.
-   Si usa CLion, en la Run/Debug Configuration ponga `Working directory` al directorio del proyecto.

## Control y uso (ejemplo de `practicas/B2T1.cpp`)

-   `J` / `L` : Mover figura horizontalmente.
-   `A` / `D` : Intercambiar texturas.
-   `R` : Resetear posición a (0,0).
-   `ESC` : Salir.

## Archivos importantes a revisar

-   `practicas/B2T1.cpp` — Ejemplo completo de manejo de texturas, VBO/VAO y entradas de teclado.
-   `practicas/shaders/` — Shaders vertex/fragment para las prácticas.
-   `external/glad`, `external/glfw`, `external/glm` — Implementaciones de dependencias.
-   `learnopengl/shader_s.h` — Clase utilitaria de manejo de shaders.
-   `learnopengl/stb_image.h` — Carga de imágenes.

## Solución de problemas comunes

-   "Failed to initialize GLAD": verificar que el contexto de GLFW esté creado antes de llamar a `gladLoadGLLoader`.
-   "Failed to load texture": comprobar ruta del archivo y `Working directory`. Habilitar `stbi_set_flip_vertically_on_load(true)` si la imagen aparece invertida.
-   Errores de compilación con OpenGL/GLFW: asegurarse de enlazar las bibliotecas correctas y usar el generador CMake adecuado para su toolchain.
-   Shaders no se ven: revisar rutas, mensajes de compilación de shader en la consola y utilizar rutas absolutas para depuración.

## Buenas prácticas

-   Mantener shaders y texturas en `practicas/` para evitar problemas con rutas relativas.
-   Configurar CLion para usar el mismo directorio de trabajo que CMake genera (project root).
-   Usar CMake para gestionar dependencias externas y opciones de build.

## Contribuciones y contacto

-   Abrir issues o pull requests en [https://github.com/Francis1918/Computacion_Grafica_Clion.git](https://github.com/Francis1918/Computacion_Grafica_Clion.git)
-   Autor de las prácticas: Francis Bravo (usuario GitHub: `Francis1918`)

## Licencia

-   Revisar archivos `LICENSE.md` en submódulos (por ejemplo `external/glfw/LICENSE.md`). Añadir aquí la licencia del proyecto si aplica.