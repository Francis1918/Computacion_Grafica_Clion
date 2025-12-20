#version 330 core
// Atributos de entrada
layout (location = 0) in vec3 aPos;      // Posicion del vertice
layout (location = 1) in vec2 aTexCoord; // Coordenadas de textura

// Salida hacia el fragment shader
out vec2 TexCoord;

// Matrices de transformacion
uniform mat4 model;      // Matriz de modelo (posicion/rotacion del objeto)
uniform mat4 view;       // Matriz de vista (camara)
uniform mat4 projection; // Matriz de proyeccion (perspectiva)

void main()
{
    // Calcular posicion final del vertice aplicando MVP
    gl_Position = projection * view * model * vec4(aPos, 1.0);

    // Pasar coordenadas de textura al fragment shader
    TexCoord = aTexCoord;
}