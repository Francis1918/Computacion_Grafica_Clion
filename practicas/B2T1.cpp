#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader_s.h>
#include <stb_image.h>
#include <iostream>
#include <cmath>

// Prototipos
static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void processInput(GLFWwindow* window);

// Constantes
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Variables globales para texturas y movimiento
static float posX = 0.0f;
static float posY = 0.0f;
static unsigned int texture1, texture2;
static bool texSwap = false;

void B2T1()
{
    // --- Inicialización ---
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "B2T1 - Francis Bravo 1726296815", NULL, NULL);
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

    // Cargar shaders con validación
    Shader ourShader("../practicas/shaders/shader_B2T1.vs", "../practicas/shaders/shader_B2T1.fs");


    // --- CARGAR TEXTURAS CON SOPORTE PARA CANAL ALFA ---
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load("../practicas/textures/BlueColor.PNG", &width, &height, &nrChannels, 0);
    if (data)
    {
        GLenum format = nrChannels == 4 ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture 1" << std::endl;
    }
    stbi_image_free(data);

    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load("../practicas/textures/CarbonFiber.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        GLenum format = nrChannels == 4 ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture 2" << std::endl;
    }
    stbi_image_free(data);

    // --- Definición de Vértices con coordenadas UV estándar ---
    float vertices[] = {
        // x, y, z    // u, v    // groupID
        // Grupo 0 (Triángulos 1-3)
        0.15f,  0.8f, 0.0f,   1.0f, 0.0f,   0.0f,
       -0.15f,  0.8f, 0.0f,   0.0f, 0.0f,   0.0f,
        0.0f,   0.95f, 0.0f,   0.5f, 1.0f,   0.0f,

        0.15f,  0.8f, 0.0f,   1.0f, 0.0f,   0.0f,
       -0.15f,  0.8f, 0.0f,   0.0f, 0.0f,   0.0f,
        0.15f,  0.4f, 0.0f,   1.0f, 1.0f,   0.0f,

        0.15f,  0.4f, 0.0f,   1.0f, 0.0f,   0.0f,
       -0.15f,  0.8f, 0.0f,   0.0f, 0.0f,   0.0f,
       -0.15f,  0.0f, 0.0f,   0.0f, 1.0f,   0.0f,

        // Grupo 1 (Triángulos 4-7)
        0.15f,  0.4f, 0.0f,   1.0f, 0.0f,   1.0f,
       -0.15f,  0.0f, 0.0f,   0.0f, 0.0f,   1.0f,
        0.15f, -0.4f, 0.0f,   1.0f, 1.0f,   1.0f,

        0.15f, -0.4f, 0.0f,   1.0f, 0.0f,   1.0f,
        0.0f,  -0.2f, 0.0f,   0.5f, 0.0f,   1.0f,
        0.0f,  -0.6f, 0.0f,   0.5f, 1.0f,   1.0f,

        0.0f,  -0.2f, 0.0f,   1.0f, 0.0f,   1.0f,
       -0.15f,  0.0f, 0.0f,   0.0f, 0.0f,   1.0f,
       -0.15f, -0.4f, 0.0f,   0.0f, 1.0f,   1.0f,

        0.0f,  -0.2f, 0.0f,   1.0f, 0.0f,   1.0f,
       -0.15f, -0.4f, 0.0f,   0.0f, 0.0f,   1.0f,
        0.0f,  -0.6f, 0.0f,   0.5f, 1.0f,   1.0f,

        0.10f, -0.75f, 0.0f,   1.0f, 0.0f,   1.0f,
       -0.10f, -0.75f, 0.0f,   0.0f, 0.0f,   1.0f,
        0.10f, -0.95f, 0.0f,   1.0f, 1.0f,   1.0f,

       -0.10f, -0.75f, 0.0f,   0.0f, 0.0f,   1.0f,
        0.10f, -0.95f, 0.0f,   1.0f, 1.0f,   1.0f,
       -0.10f, -0.95f, 0.0f,   0.0f, 1.0f,   1.0f
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Posición
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // UV
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // Grupo
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // --- Configuración de texturas (solo una vez para optimizar CPU/GPU) ---
    ourShader.use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);

    // --- Bucle de renderizado ---
    std::cout << "\n=== CONTROLES ===" << std::endl;
    std::cout << "J/L: Mover figura horizontalmente" << std::endl;
    std::cout << "A/D: Intercambiar texturas" << std::endl;
    std::cout << "R: Resetear posicion" << std::endl;
    std::cout << "ESC: Salir\n" << std::endl;

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Enviar uniforms a GPU (optimizado: solo valores que cambian)
        ourShader.setVec2("positionOffset", posX, posY);
        ourShader.setBool("texSwap", texSwap);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 27);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteTextures(1, &texture1);
    glDeleteTextures(1, &texture2);
    glfwTerminate();
}

static void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Control de velocidad basado en tiempo
    static float lastFrame = 0.0f;
    float currentFrame = glfwGetTime();
    float deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    float moveSpeed = 0.5f * deltaTime; // Velocidad ajustada con deltaTime

    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
        posX -= moveSpeed;
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        posX += moveSpeed;

    static bool aPressed = false;
    static bool dPressed = false;

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && !aPressed)
    {
        texSwap = !texSwap;
        aPressed = true;
        std::cout << "Texturas intercambiadas: " << (texSwap ? "SI" : "NO") << std::endl;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE)
        aPressed = false;

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && !dPressed)
    {
        texSwap = !texSwap;
        dPressed = true;
        std::cout << "Texturas intercambiadas: " << (texSwap ? "SI" : "NO") << std::endl;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE)
        dPressed = false;

    // Resetear posición
    static bool rPressed = false;
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && !rPressed)
    {
        posX = 0.0f;
        posY = 0.0f;
        rPressed = true;
        std::cout << "Posicion reseteada a (0, 0)" << std::endl;
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_RELEASE)
        rPressed = false;
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}