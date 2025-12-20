//
// Created by bravo on 19/12/2025.
//
//
// Francis Bravo 1726296815
// Escena tipo Minecraft con piso y personaje (cabeza de Fry)
//
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"

#include "stb_image.h"

#include <iostream>

// Callbacks
static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
static void processInput(GLFWwindow *window);

// Configuracion de ventana
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Camara
static Camera camera(glm::vec3(0.0f, 2.0f, 8.0f));
static float lastX = SCR_WIDTH / 2.0f;
static float lastY = SCR_HEIGHT / 2.0f;
static bool firstMouse = true;

// Timing
static float deltaTime = 0.0f;
static float lastFrame = 0.0f;

void B2T3()
{
    // Inicializar GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Crear ventana
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Francis Bravo 1726296815", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Capturar mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Cargar GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }

    // Habilitar depth test para 3D
    glEnable(GL_DEPTH_TEST);

    // Compilar shaders
    Shader ourShader("../practicas/shaders/B2T3.vs", "../practicas/shaders/B2T3.fs");

    // ============================================================
    // VERTICES DEL CUBO COMPLETO PARA EL PISO (textura pngwing.com.png)
    // Imagen desplegada de cubo de mineral - evitando bordes negros
    // Se aplica un margen interno (padding) MAS AGRESIVO para no tomar las lineas negras
    // ============================================================

    // Coordenadas UV para cada cara del cubo de mineral (con padding extra)
    // Cara 1 (Arriba - TOP)
    float topU1 = 0.32f, topU2 = 0.43f;
    float topV1 = 0.80f, topV2 = 0.91f;

    // Cara 2 (Frente - FRONT)
    float frontU1 = 0.32f, frontU2 = 0.43f;
    float frontV1 = 0.57f, frontV2 = 0.66f;

    // Cara 3 (Izquierda - LEFT)
    float leftU1 = 0.07f, leftU2 = 0.18f;
    float leftV1 = 0.57f, leftV2 = 0.66f;

    // Cara 4 (Derecha - RIGHT)
    float rightU1 = 0.57f, rightU2 = 0.68f;
    float rightV1 = 0.57f, rightV2 = 0.66f;

    // Cara 5 (Atras - BACK)
    float backU1 = 0.82f, backU2 = 0.93f;
    float backV1 = 0.57f, backV2 = 0.66f;

    // Cara 6 (Abajo - BOTTOM)
    float bottomU1 = 0.32f, bottomU2 = 0.43f;
    float bottomV1 = 0.32f, bottomV2 = 0.43f;

    float floorVertices[] = {
        // positions          // texture coords
        // Cara superior (TOP) - visible desde arriba
        -0.5f,  0.5f, -0.5f,  topU1, topV1,
         0.5f,  0.5f, -0.5f,  topU2, topV1,
         0.5f,  0.5f,  0.5f,  topU2, topV2,
         0.5f,  0.5f,  0.5f,  topU2, topV2,
        -0.5f,  0.5f,  0.5f,  topU1, topV2,
        -0.5f,  0.5f, -0.5f,  topU1, topV1,

        // Cara inferior (BOTTOM)
        -0.5f, -0.5f, -0.5f,  bottomU1, bottomV2,
         0.5f, -0.5f, -0.5f,  bottomU2, bottomV2,
         0.5f, -0.5f,  0.5f,  bottomU2, bottomV1,
         0.5f, -0.5f,  0.5f,  bottomU2, bottomV1,
        -0.5f, -0.5f,  0.5f,  bottomU1, bottomV1,
        -0.5f, -0.5f, -0.5f,  bottomU1, bottomV2,

        // Cara frontal (FRONT)
        -0.5f, -0.5f,  0.5f,  frontU1, frontV1,
         0.5f, -0.5f,  0.5f,  frontU2, frontV1,
         0.5f,  0.5f,  0.5f,  frontU2, frontV2,
         0.5f,  0.5f,  0.5f,  frontU2, frontV2,
        -0.5f,  0.5f,  0.5f,  frontU1, frontV2,
        -0.5f, -0.5f,  0.5f,  frontU1, frontV1,

        // Cara trasera (BACK)
        -0.5f, -0.5f, -0.5f,  backU2, backV1,
         0.5f, -0.5f, -0.5f,  backU1, backV1,
         0.5f,  0.5f, -0.5f,  backU1, backV2,
         0.5f,  0.5f, -0.5f,  backU1, backV2,
        -0.5f,  0.5f, -0.5f,  backU2, backV2,
        -0.5f, -0.5f, -0.5f,  backU2, backV1,

        // Cara izquierda (LEFT)
        -0.5f, -0.5f, -0.5f,  leftU1, leftV1,
        -0.5f, -0.5f,  0.5f,  leftU2, leftV1,
        -0.5f,  0.5f,  0.5f,  leftU2, leftV2,
        -0.5f,  0.5f,  0.5f,  leftU2, leftV2,
        -0.5f,  0.5f, -0.5f,  leftU1, leftV2,
        -0.5f, -0.5f, -0.5f,  leftU1, leftV1,

        // Cara derecha (RIGHT)
         0.5f, -0.5f, -0.5f,  rightU2, rightV1,
         0.5f, -0.5f,  0.5f,  rightU1, rightV1,
         0.5f,  0.5f,  0.5f,  rightU1, rightV2,
         0.5f,  0.5f,  0.5f,  rightU1, rightV2,
         0.5f,  0.5f, -0.5f,  rightU2, rightV2,
         0.5f, -0.5f, -0.5f,  rightU2, rightV1,
    };

    // ============================================================
    // VERTICES DEL CUBO PARA LA CABEZA DE FRY (Texture3.png)
    // Coordenadas UV calculadas del cubeecraft de Fry
    // Se aplica un margen interno (padding) para evitar las lineas negras de corte
    // ============================================================

    // Margen para recortar bordes negros (padding) - MUY AUMENTADO
    float pad = 0.050f;  // 5% de margen interno

    // Frente (cara con ojos)
    float fU1 = 0.555f + pad, fU2 = 0.720f - pad;
    float fV1 = 0.535f + pad, fV2 = 0.765f - pad;

    // Arriba (pelo naranja)
    float tU1 = 0.555f + pad, tU2 = 0.720f - pad;
    float tV1 = 0.765f + pad, tV2 = 0.995f - pad;

    // Abajo (cuello/barbilla)
    float boU1 = 0.555f + pad, boU2 = 0.720f - pad;
    float boV1 = 0.305f + pad, boV2 = 0.535f - pad;

    // Izquierda (oreja izquierda)
    float lU1 = 0.390f + pad, lU2 = 0.555f - pad;
    float lV1 = 0.535f + pad, lV2 = 0.765f - pad;

    // Derecha (oreja derecha)
    float rU1 = 0.720f + pad, rU2 = 0.885f - pad;
    float rV1 = 0.535f + pad, rV2 = 0.765f - pad;

    // Atras (parte trasera de la cabeza)
    float bU1 = 0.555f + pad, bU2 = 0.720f - pad;
    float bV1 = 0.075f + pad, bV2 = 0.305f - pad;

    float headVertices[] = {
        // positions          // texture coords
        // Cara trasera (atras de la cabeza)
        -0.5f, -0.5f, -0.5f,  bU2, bV1,
         0.5f, -0.5f, -0.5f,  bU1, bV1,
         0.5f,  0.5f, -0.5f,  bU1, bV2,
         0.5f,  0.5f, -0.5f,  bU1, bV2,
        -0.5f,  0.5f, -0.5f,  bU2, bV2,
        -0.5f, -0.5f, -0.5f,  bU2, bV1,

        // Cara frontal (cara de Fry con ojos)
        -0.5f, -0.5f,  0.5f,  fU2, fV1,
         0.5f, -0.5f,  0.5f,  fU1, fV1,
         0.5f,  0.5f,  0.5f,  fU1, fV2,
         0.5f,  0.5f,  0.5f,  fU1, fV2,
        -0.5f,  0.5f,  0.5f,  fU2, fV2,
        -0.5f, -0.5f,  0.5f,  fU2, fV1,

        // Cara izquierda (oreja izquierda)
        -0.5f,  0.5f,  0.5f,  lU1, lV2,
        -0.5f,  0.5f, -0.5f,  lU2, lV2,
        -0.5f, -0.5f, -0.5f,  lU2, lV1,
        -0.5f, -0.5f, -0.5f,  lU2, lV1,
        -0.5f, -0.5f,  0.5f,  lU1, lV1,
        -0.5f,  0.5f,  0.5f,  lU1, lV2,

        // Cara derecha (oreja derecha)
         0.5f,  0.5f,  0.5f,  rU2, rV2,
         0.5f,  0.5f, -0.5f,  rU1, rV2,
         0.5f, -0.5f, -0.5f,  rU1, rV1,
         0.5f, -0.5f, -0.5f,  rU1, rV1,
         0.5f, -0.5f,  0.5f,  rU2, rV1,
         0.5f,  0.5f,  0.5f,  rU2, rV2,

        // Cara inferior (cuello/base)
        -0.5f, -0.5f, -0.5f,  boU2, boV2,
         0.5f, -0.5f, -0.5f,  boU1, boV2,
         0.5f, -0.5f,  0.5f,  boU1, boV1,
         0.5f, -0.5f,  0.5f,  boU1, boV1,
        -0.5f, -0.5f,  0.5f,  boU2, boV1,
        -0.5f, -0.5f, -0.5f,  boU2, boV2,

        // Cara superior (pelo)
        -0.5f,  0.5f, -0.5f,  tU2, tV1,
         0.5f,  0.5f, -0.5f,  tU1, tV1,
         0.5f,  0.5f,  0.5f,  tU1, tV2,
         0.5f,  0.5f,  0.5f,  tU1, tV2,
        -0.5f,  0.5f,  0.5f,  tU2, tV2,
        -0.5f,  0.5f, -0.5f,  tU2, tV1,
    };

    // ============================================================
    // Configurar VAO/VBO para el piso
    // ============================================================
    unsigned int floorVAO, floorVBO;
    glGenVertexArrays(1, &floorVAO);
    glGenBuffers(1, &floorVBO);

    glBindVertexArray(floorVAO);
    glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(floorVertices), floorVertices, GL_STATIC_DRAW);

    // Atributo de posicion
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Atributo de coordenadas de textura
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // ============================================================
    // Configurar VAO/VBO para la cabeza
    // ============================================================
    unsigned int headVAO, headVBO;
    glGenVertexArrays(1, &headVAO);
    glGenBuffers(1, &headVBO);

    glBindVertexArray(headVAO);
    glBindBuffer(GL_ARRAY_BUFFER, headVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(headVertices), headVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // ============================================================
    // Cargar textura del piso (pngwing.com.png - bloque de mineral)
    // ============================================================
    unsigned int textureFloor;
    glGenTextures(1, &textureFloor);
    glBindTexture(GL_TEXTURE_2D, textureFloor);

    // Parametros de wrapping y filtrado
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("../practicas/textures/pngwing.com.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        // PNG con canal alpha
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load floor texture" << std::endl;
    }
    stbi_image_free(data);

    // ============================================================
    // Cargar textura de la cabeza (Texture3.png - Fry)
    // ============================================================
    unsigned int textureHead;
    glGenTextures(1, &textureHead);
    glBindTexture(GL_TEXTURE_2D, textureHead);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load("../practicas/textures/Texture3.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load head texture" << std::endl;
    }
    stbi_image_free(data);

    // Configurar shader
    ourShader.use();
    ourShader.setInt("texture1", 0);

    // ============================================================
    // Posiciones de los cubos del piso (grid de 10x10)
    // ============================================================
    const int FLOOR_SIZE = 10;

    // ============================================================
    // RENDER LOOP
    // ============================================================
    while (!glfwWindowShouldClose(window))
    {
        // Calcular delta time
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Procesar input
        processInput(window);

        // Limpiar buffers
        glClearColor(0.5f, 0.7f, 0.9f, 1.0f);  // Color cielo azul claro
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Activar shader
        ourShader.use();

        // Matriz de proyeccion
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),
                                                (float)SCR_WIDTH / (float)SCR_HEIGHT,
                                                0.1f, 100.0f);
        ourShader.setMat4("projection", projection);

        // Matriz de vista (camara)
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("view", view);

        // ============================================================
        // Dibujar el piso (grid de cubos)
        // ============================================================
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureFloor);
        glBindVertexArray(floorVAO);

        for (int x = -FLOOR_SIZE/2; x < FLOOR_SIZE/2; x++)
        {
            for (int z = -FLOOR_SIZE/2; z < FLOOR_SIZE/2; z++)
            {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3((float)x, -0.5f, (float)z));
                ourShader.setMat4("model", model);
                glDrawArrays(GL_TRIANGLES, 0, 36);  // 6 caras * 6 vertices = cubo completo
            }
        }

        // ============================================================
        // Dibujar la cabeza de Fry sobre el piso
        // ============================================================
        glBindTexture(GL_TEXTURE_2D, textureHead);
        glBindVertexArray(headVAO);

        glm::mat4 model = glm::mat4(1.0f);
        // Posicionar la cabeza sobre el piso (y = 0.5 para que este encima del piso)
        model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
        ourShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);  // 6 caras * 6 vertices = cubo completo

        // Swap buffers y poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Liberar recursos
    glDeleteVertexArrays(1, &floorVAO);
    glDeleteBuffers(1, &floorVBO);
    glDeleteVertexArrays(1, &headVAO);
    glDeleteBuffers(1, &headVBO);

    glfwTerminate();
    return;
}

// Procesar input del teclado
static void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Movimiento WASD
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// Callback para redimensionar ventana
static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// Callback para movimiento del mouse
static void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// Callback para scroll del mouse (zoom)
static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}