#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader_s.h>
#include <iostream>
#include <cmath>

// Prototipos
static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void processInput(GLFWwindow* window);

// Constantes
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Variables globales para animación
static float posX = 0.0f;
static float posY = 0.0f;
static float colorR = 1.0f;
static float colorG = 1.0f;
static float colorB = 1.0f;
static bool animateWithTime = false;

void B1P2()
{
    // --- Inicialización ---
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Francis Bravo 1726296815", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }

    Shader ourShader("../practicas/shaders/shader_B1P2.vs", "../practicas/shaders/shader_B1P2.fs");

    // --- Definición de Vértices (MODO ESPEJO) ---
    // Se ha invertido el signo de todas las coordenadas X.
    // Ejemplo: -0.15f pasa a ser 0.15f y viceversa.

    float vertices[] = {
        // x, y, z            // r, g, b  // group

        // 1. Triángulo Superior (Rojo) - group 0
         0.15f,  0.8f, 0.0f,  1.0f, 0.0f, 0.0f, 0.0f,
        -0.15f,  0.8f, 0.0f,  1.0f, 0.0f, 0.0f, 0.0f,
         0.0f,   0.95f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,

        // 2. Triángulo Derecho Superior (Rosa) - group 1
         0.15f,  0.8f, 0.0f,  1.0f, 0.6f, 0.8f, 1.0f,
        -0.15f,  0.8f, 0.0f,  1.0f, 0.6f, 0.8f, 1.0f,
         0.15f,  0.4f, 0.0f,  1.0f, 0.6f, 0.8f, 1.0f,

        // 3. Triángulo Izquierdo Grande (Naranja) - group 2
         0.15f,  0.4f, 0.0f,  1.0f, 0.6f, 0.0f, 2.0f,
        -0.15f,  0.8f, 0.0f,  1.0f, 0.6f, 0.0f, 2.0f,
        -0.15f,  0.0f, 0.0f,  1.0f, 0.6f, 0.0f, 2.0f,

        // 4. Triángulo Central Grande (Azul) - group 3
         0.15f,  0.4f, 0.0f,  0.0f, 0.6f, 1.0f, 3.0f,
        -0.15f,  0.0f, 0.0f,  0.0f, 0.6f, 1.0f, 3.0f,
         0.15f, -0.4f, 0.0f,  0.0f, 0.6f, 1.0f, 3.0f,

        // 5. Triángulo Inferior Derecho (Morado) - group 4
         0.15f, -0.4f, 0.0f,  0.6f, 0.3f, 0.8f, 4.0f,
         0.0f,  -0.2f, 0.0f,  0.6f, 0.3f, 0.8f, 4.0f,
         0.0f,  -0.6f, 0.0f,  0.6f, 0.3f, 0.8f, 4.0f,

        // 6. Paralelogramo Inferior Izquierdo (Amarillo) - group 5
        // Triángulo A
         0.0f,  -0.2f, 0.0f,  1.0f, 1.0f, 0.0f, 5.0f,
        -0.15f,  0.0f, 0.0f,  1.0f, 1.0f, 0.0f, 5.0f,
        -0.15f, -0.4f, 0.0f,  1.0f, 1.0f, 0.0f, 5.0f,
        // Triángulo B
         0.0f,  -0.2f, 0.0f,  1.0f, 1.0f, 0.0f, 5.0f,
        -0.15f, -0.4f, 0.0f,  1.0f, 1.0f, 0.0f, 5.0f,
         0.0f,  -0.6f, 0.0f,  1.0f, 1.0f, 0.0f, 5.0f,

        // 7. Cuadrado del punto (Verde) - group 6
        // Triángulo 1
         0.10f, -0.75f, 0.0f, 0.0f, 1.0f, 0.0f, 6.0f,
        -0.10f, -0.75f, 0.0f, 0.0f, 1.0f, 0.0f, 6.0f,
         0.10f, -0.95f, 0.0f, 0.0f, 1.0f, 0.0f, 6.0f,
        // Triángulo 2
        -0.10f, -0.75f, 0.0f, 0.0f, 1.0f, 0.0f, 6.0f,
         0.10f, -0.95f, 0.0f, 0.0f, 1.0f, 0.0f, 6.0f,
        -0.10f, -0.95f, 0.0f, 0.0f, 1.0f, 0.0f, 6.0f
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Posición
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // Grupo
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // --- Bucle de renderizado ---
    std::cout << "\n=== CONTROLES ===" << std::endl;
    std::cout << "WASD: Mover figura" << std::endl;
    std::cout << "R/T: Aumentar/Disminuir rojo" << std::endl;
    std::cout << "G/H: Aumentar/Disminuir verde" << std::endl;
    std::cout << "B/N: Aumentar/Disminuir azul" << std::endl;
    std::cout << "ESPACIO: Activar/Desactivar animacion automatica" << std::endl;
    std::cout << "C: Reset" << std::endl;
    std::cout << "ESC: Salir\n" << std::endl;

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ourShader.use();

        // --- Aplicar valores de tiempo y transformaciones ---
        float timeValue = glfwGetTime();

        // Si la animación automática está activada, modifica posición/color
        float finalPosX = posX;
        float finalPosY = posY;
        float finalColorR = colorR;
        float finalColorG = colorG;
        float finalColorB = colorB;

        if (animateWithTime)
        {
            // Animación de movimiento circular
            finalPosX += sin(timeValue) * 0.3f;
            finalPosY += cos(timeValue) * 0.3f;

            // Animación de color ciclico para toda la gama
            float hue = timeValue * 0.5f; // Velocidad del ciclo de colores
            finalColorR = (sin(hue) + 1.0f) / 2.0f;
            finalColorG = (sin(hue + 2.0944f) + 1.0f) / 2.0f; // 120 grados
            finalColorB = (sin(hue + 4.1888f) + 1.0f) / 2.0f; // 240 grados
        }

        // Enviar uniforms al shader
        ourShader.setFloat("timeValue", timeValue);
        ourShader.setVec2("positionOffset", finalPosX, finalPosY);
        ourShader.setVec3("colorModifier", finalColorR, finalColorG, finalColorB);
        ourShader.setBool("isAnimating", animateWithTime);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 27);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
}

static void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // --- CONTROL DE POSICIÓN ---
    float moveSpeed = 0.001f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        posY += moveSpeed;  // Arriba
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        posY -= moveSpeed;  // Abajo
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        posX -= moveSpeed;  // Izquierda
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        posX += moveSpeed;  // Derecha

    // --- CONTROL DE COLOR ---
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        colorR = (colorR < 1.0f) ? colorR + 0.01f : 1.0f;  // Aumentar rojo
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
        colorR = (colorR > 0.0f) ? colorR - 0.01f : 0.0f;  // Disminuir rojo

    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
        colorG = (colorG < 1.0f) ? colorG + 0.01f : 1.0f;  // Aumentar verde
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
        colorG = (colorG > 0.0f) ? colorG - 0.01f : 0.0f;  // Disminuir verde

    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
        colorB = (colorB < 1.0f) ? colorB + 0.01f : 1.0f;  // Aumentar azul
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
        colorB = (colorB > 0.0f) ? colorB - 0.01f : 0.0f;  // Disminuir azul

    // --- TOGGLE ANIMACIÓN AUTOMÁTICA ---
    static bool spacePressed = false;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !spacePressed)
    {
        animateWithTime = !animateWithTime;
        spacePressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
        spacePressed = false;

    // --- RESET ---
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
    {
        posX = 0.0f;
        posY = 0.0f;
        colorR = colorG = colorB = 1.0f;
        animateWithTime = false;
    }
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}