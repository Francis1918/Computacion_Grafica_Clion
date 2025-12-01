//
// Created by bravo on 17/11/2025.
//
#include <glad/glad.h>      // GLAD: carga las funciones de OpenGL
#include <GLFW/glfw3.h>     // GLFW: manejo de ventanas y entrada de usuario
#include <iostream>         // Para usar cout y mostrar mensajes en consola
#include <cmath>            // Para usar funciones matematicas como cos y sin

// Declaracion de funciones que se definen mas abajo y sin static
static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void processInput(GLFWwindow* window);

// Constantes para definir el tamaño de la ventana
const unsigned int SCR_WIDTH = 800;   // Ancho de la ventana en pixeles
const unsigned int SCR_HEIGHT = 600;  // Alto de la ventana en pixeles

// Vertex Shader: procesa cada vertice
// Recibe la posicion (aPos) y el color (aColor) de cada vertice
// Envia el color al fragment shader a traves de ourColor
//sin static
static const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"      // Posicion del vertice
"layout (location = 1) in vec3 aColor;\n"    // Color del vertice
"out vec3 ourColor;\n"                       // Color que se envia al fragment shader
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n"        // Establecer posicion del vertice
"   ourColor = aColor;\n"                    // Pasar el color al siguiente shader
"}\0";

// Fragment Shader: determina el color de cada pixel
// Recibe el color interpolado de los vertices
// Genera el color final del pixel
//sin static
static const char* fragmentShaderSource = "#version 330 core\n"
"in vec3 ourColor;\n"                        // Color recibido del vertex shader
"out vec4 FragColor;\n"                      // Color final del pixel
"void main()\n"
"{\n"
"   FragColor = vec4(ourColor, 1.0);\n"      // Asignar color con alpha 1.0 (opaco)
"}\0";

void B1T3()//int main()
{
    // Inicializar GLFW
    glfwInit();

    // Configurar la version de OpenGL a usar (3.3)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  // Version mayor: 3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);  // Version menor: 3

    // Usar el perfil core de OpenGL (sin funciones deprecadas)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Crear la ventana con titulo personalizado
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Francis Bravo 1726296815", NULL, NULL);
    if (window == NULL)  // Verificar si la ventana se creo correctamente
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();  // Terminar GLFW si falla
        return; //-1
    }

    // Hacer que el contexto de OpenGL de esta ventana sea el contexto actual
    glfwMakeContextCurrent(window);

    // Registrar la funcion callback para cuando se redimensione la ventana
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Cargar todas las funciones de OpenGL usando GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return; //-1
    }

    // Crear y compilar el vertex shader
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);          // Crear shader
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);   // Asignar codigo fuente
    glCompileShader(vertexShader);                                // Compilar

    // Crear y compilar el fragment shader
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);      // Crear shader
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL); // Asignar codigo fuente
    glCompileShader(fragmentShader);                              // Compilar

    // Crear el programa de shaders y enlazar los shaders compilados
    int shaderProgram = glCreateProgram();                        // Crear programa
    glAttachShader(shaderProgram, vertexShader);                  // Adjuntar vertex shader
    glAttachShader(shaderProgram, fragmentShader);                // Adjuntar fragment shader
    glLinkProgram(shaderProgram);                                 // Enlazar programa

    // Eliminar los shaders ya que estan enlazados al programa y ya no se necesitan
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Parametros del poligono
    int n = 8;          // Numero de lados del poligono (octogono)
    float r = 0.8f;     // Radio del poligono
    float PI = 3.14159f; // Valor de PI para calculos trigonometricos

    // Array de colores para cada triangulo
    // Cada triangulo tendra un color diferente
    float colors[8][3] = {
        {0.0f, 0.0f, 1.0f},  // Azul
        {1.0f, 1.0f, 0.0f},  // Amarillo
        {1.0f, 0.0f, 0.0f},  // Rojo
        {0.0f, 1.0f, 0.0f},  // Verde
        {1.0f, 0.0f, 1.0f},  // Magenta
        {0.0f, 1.0f, 1.0f},  // Cyan
        {0.0f, 0.0f, 0.0f},  // Negro
        {0.0f, 0.8f, 0.8f}   // Turquesa
    };

    // Array para almacenar todos los vertices del poligono
    // Cada triangulo tiene 3 vertices, cada vertice tiene 6 valores (x,y,z,r,g,b)
    // 8 triangulos * 3 vertices * 6 valores = 144 valores totales
    float vertices[144];

    // Generar los vertices para cada triangulo del poligono
    for (int i = 0; i < n; i++)
    {
        // Calcular el angulo para el primer vertice del triangulo
        float angle1 = 2.0f * PI * i / n;

        // Calcular el angulo para el segundo vertice del triangulo
        float angle2 = 2.0f * PI * (i + 1) / n;

        // Calcular el indice base en el array de vertices
        int index = i * 18;  // 18 valores por triangulo (3 vertices * 6 valores)

        // Primer vertice: centro del poligono (0, 0, 0)
        vertices[index + 0] = 0.0f;           // Coordenada x
        vertices[index + 1] = 0.0f;           // Coordenada y
        vertices[index + 2] = 0.0f;           // Coordenada z
        vertices[index + 3] = colors[i][0];   // Componente rojo del color
        vertices[index + 4] = colors[i][1];   // Componente verde del color
        vertices[index + 5] = colors[i][2];   // Componente azul del color

        // Segundo vertice: punto en el perimetro del circulo en angle1
        vertices[index + 6] = r * cos(angle1);   // Coordenada x usando coseno
        vertices[index + 7] = r * sin(angle1);   // Coordenada y usando seno
        vertices[index + 8] = 0.0f;              // Coordenada z
        vertices[index + 9] = colors[i][0];      // Componente rojo del color
        vertices[index + 10] = colors[i][1];     // Componente verde del color
        vertices[index + 11] = colors[i][2];     // Componente azul del color

        // Tercer vertice: punto en el perimetro del circulo en angle2
        vertices[index + 12] = r * cos(angle2);  // Coordenada x usando coseno
        vertices[index + 13] = r * sin(angle2);  // Coordenada y usando seno
        vertices[index + 14] = 0.0f;             // Coordenada z
        vertices[index + 15] = colors[i][0];     // Componente rojo del color
        vertices[index + 16] = colors[i][1];     // Componente verde del color
        vertices[index + 17] = colors[i][2];     // Componente azul del color
    }

    // Crear los objetos de OpenGL para manejar los vertices
    unsigned int VBO, VAO;  // VBO: Vertex Buffer Object, VAO: Vertex Array Object

    // Generar el VAO (almacena la configuracion de los atributos de vertices)
    glGenVertexArrays(1, &VAO);

    // Generar el VBO (almacena los datos de los vertices)
    glGenBuffers(1, &VBO);

    // Enlazar el VAO primero
    glBindVertexArray(VAO);

    // Enlazar y configurar el VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Copiar los datos de vertices al buffer de OpenGL
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Configurar el atributo de posicion (location = 0)
    // 3 valores (x,y,z), tipo float, stride de 6 floats, offset 0
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);  // Activar el atributo de posicion

    // Configurar el atributo de color (location = 1)
    // 3 valores (r,g,b), tipo float, stride de 6 floats, offset 3 floats
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);  // Activar el atributo de color

    // Bucle de renderizado
    // Se ejecuta continuamente hasta que se cierre la ventana
    while (!glfwWindowShouldClose(window))
    {
        // Procesar entrada del teclado
        processInput(window);

        // Limpiar el buffer de color con un color gris-verdoso
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Usar el programa de shaders que creamos
        glUseProgram(shaderProgram);

        // Enlazar el VAO que contiene nuestros vertices
        glBindVertexArray(VAO);

        // Dibujar los triangulos
        // GL_TRIANGLES: modo de dibujo, 0: indice inicial, 24: numero de vertices (8 triangulos * 3 vertices)
        glDrawArrays(GL_TRIANGLES, 0, 24);

        // Intercambiar los buffers de la ventana (double buffering)
        glfwSwapBuffers(window);

        // Procesar eventos de ventana (teclado, mouse, etc.)
        glfwPollEvents();
    }

    // Limpiar todos los recursos asignados
    glDeleteVertexArrays(1, &VAO);     // Eliminar VAO
    glDeleteBuffers(1, &VBO);          // Eliminar VBO
    glDeleteProgram(shaderProgram);    // Eliminar programa de shaders

    // Terminar GLFW y liberar recursos
    glfwTerminate();
    return; //0
}

// Funcion para procesar la entrada del teclado y sin static
static void processInput(GLFWwindow* window)
{
    // Si se presiona la tecla ESC, cerrar la ventana
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// Funcion callback que se ejecuta cuando se redimensiona la ventana y sin static
static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // Ajustar el viewport de OpenGL al nuevo tamaño de la ventana
    glViewport(0, 0, width, height);
}