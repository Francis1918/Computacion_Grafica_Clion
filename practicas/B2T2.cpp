//
// Created by bravo on 14/12/2025.
//
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
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);//ajusta el tamaño de la ventana

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {//carga la funciones de la libreria glad
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }

    // Cargar shaders con validación
    Shader ourShader("../practicas/shaders/shader_B2T1.vs", "../practicas/shaders/shader_B2T1.fs");
    //construye el objeto shader con las rutas de los archivos de shader vs y fs

    // --- CARGAR TEXTURAS CON SOPORTE PARA CANAL ALFA ---
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);// crean y vinculan el objeto de textura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //configuran el wrapping junto con los filtros repeat y linear respectivamente
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);//voltea la imagen al cargarla en las coordenadad UV
    unsigned char *data = stbi_load("../practicas/textures/BlueColor.PNG", &width, &height, &nrChannels, 0);
    //obtiene las rutas de las texturas y las carga en memoria
    if (data)
    {
        GLenum format = nrChannels == 4 ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } //carga la texturas si son validas
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
    //coodenadas de la figura compuesta por dos grupos de triangulos para las texturas
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
    //sube todos los vetices a la GPU
    // Posición
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // UV
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // Grupo
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);
    // Configura los atributos de los vértices: posición, coordenadas UV y grupoID
    // --- Configuración de texturas (solo una vez para optimizar CPU/GPU) ---
    ourShader.use();//activa el programa shader
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);
    //decirle al shader que unidades de textura usar
    // --- Bucle de renderizado ---
    //imprimir en consola la guia de uso de programa (controles)
    std::cout << "\n=== CONTROLES ===" << std::endl;
    std::cout << "J/L: Mover figura horizontalmente" << std::endl;
    std::cout << "A/D: Intercambiar texturas" << std::endl;
    std::cout << "R: Resetear posicion" << std::endl;
    std::cout << "ESC: Salir\n" << std::endl;

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);//procesa las entradas que recibe por el teclado

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);//limpian la pantalla con el color especificado

        // Enviar uniforms a GPU (optimizado: solo valores que cambian)
        ourShader.setVec2("positionOffset", posX, posY);//reset
        ourShader.setBool("texSwap", texSwap);//decide que shade usar

        glBindVertexArray(VAO);//dibuja los 27 vertices de cada triangulo
        glDrawArrays(GL_TRIANGLES, 0, 27);

        glfwSwapBuffers(window);
        glfwPollEvents();//muestran el frame y procesan eventos
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteTextures(1, &texture1);
    glDeleteTextures(1, &texture2);
    glfwTerminate();
    //libera los recursos de la GPU y cierra la ventana
}

static void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);// usa la tecla esc para cerrar la ventana

    // Control de velocidad basado en tiempo presionando las teclas
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