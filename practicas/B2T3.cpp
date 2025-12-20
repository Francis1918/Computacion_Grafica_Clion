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

static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
static void processInput(GLFWwindow *window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

static Camera camera(glm::vec3(0.0f, 2.0f, 8.0f));
static float lastX = SCR_WIDTH / 2.0f;
static float lastY = SCR_HEIGHT / 2.0f;
static bool firstMouse = true;

static float deltaTime = 0.0f;
static float lastFrame = 0.0f;

void B2T3()
{
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
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }

    glEnable(GL_DEPTH_TEST);

    Shader ourShader("../practicas/shaders/shader_B2T3.vs", "../practicas/shaders/shader_B2T3.fs");

    // ============================================================
    // COORDENADAS UV PARA EL PISO (pngwing.com.png - 1482x1173)
    // Cara X central del cubo desplegado: x=370-617, y=370-617
    // ============================================================
    float floorU1 = 370.0f / 1482.0f;   // 0.2496
    float floorU2 = 617.0f / 1482.0f;   // 0.4163
    float floorV1 = 1.0f - (617.0f / 1173.0f);  // 0.4740 (invertido Y)
    float floorV2 = 1.0f - (370.0f / 1173.0f);  // 0.6846

    float floorVertices[] = {
        // Cara superior (visible desde arriba)
        -0.5f, 0.0f, -0.5f,   floorU1, floorV1,
         0.5f, 0.0f, -0.5f,   floorU2, floorV1,
         0.5f, 0.0f,  0.5f,   floorU2, floorV2,
         0.5f, 0.0f,  0.5f,   floorU2, floorV2,
        -0.5f, 0.0f,  0.5f,   floorU1, floorV2,
        -0.5f, 0.0f, -0.5f,   floorU1, floorV1,

        // Cara frontal
        -0.5f, -0.5f,  0.5f,  floorU1, floorV1,
         0.5f, -0.5f,  0.5f,  floorU2, floorV1,
         0.5f,  0.0f,  0.5f,  floorU2, floorV2,
         0.5f,  0.0f,  0.5f,  floorU2, floorV2,
        -0.5f,  0.0f,  0.5f,  floorU1, floorV2,
        -0.5f, -0.5f,  0.5f,  floorU1, floorV1,

        // Cara trasera
        -0.5f, -0.5f, -0.5f,  floorU2, floorV1,
         0.5f, -0.5f, -0.5f,  floorU1, floorV1,
         0.5f,  0.0f, -0.5f,  floorU1, floorV2,
         0.5f,  0.0f, -0.5f,  floorU1, floorV2,
        -0.5f,  0.0f, -0.5f,  floorU2, floorV2,
        -0.5f, -0.5f, -0.5f,  floorU2, floorV1,

        // Cara izquierda
        -0.5f, -0.5f, -0.5f,  floorU1, floorV1,
        -0.5f, -0.5f,  0.5f,  floorU2, floorV1,
        -0.5f,  0.0f,  0.5f,  floorU2, floorV2,
        -0.5f,  0.0f,  0.5f,  floorU2, floorV2,
        -0.5f,  0.0f, -0.5f,  floorU1, floorV2,
        -0.5f, -0.5f, -0.5f,  floorU1, floorV1,

        // Cara derecha
         0.5f, -0.5f, -0.5f,  floorU2, floorV1,
         0.5f, -0.5f,  0.5f,  floorU1, floorV1,
         0.5f,  0.0f,  0.5f,  floorU1, floorV2,
         0.5f,  0.0f,  0.5f,  floorU1, floorV2,
         0.5f,  0.0f, -0.5f,  floorU2, floorV2,
         0.5f, -0.5f, -0.5f,  floorU2, floorV1,
    };

    // ============================================================
    // COORDENADAS UV PARA LA CABEZA DE FRY (Texture3.png - 1320x1020)
    // Seccion HEAD a la derecha de la imagen
    // ============================================================

    // Frente (cara con ojos): x=627-867, y=248-488
    float fU1 = 627.0f / 1320.0f;   // 0.475
    float fU2 = 867.0f / 1320.0f;   // 0.657
    float fV1 = 1.0f - (488.0f / 1020.0f);  // 0.522
    float fV2 = 1.0f - (248.0f / 1020.0f);  // 0.757

    // Arriba (pelo naranja): x=627-867, y=88-248
    float tU1 = 627.0f / 1320.0f;
    float tU2 = 867.0f / 1320.0f;
    float tV1 = 1.0f - (248.0f / 1020.0f);  // 0.757
    float tV2 = 1.0f - (88.0f / 1020.0f);   // 0.914

    // Abajo (cuello): x=627-867, y=488-648
    float boU1 = 627.0f / 1320.0f;
    float boU2 = 867.0f / 1320.0f;
    float boV1 = 1.0f - (648.0f / 1020.0f); // 0.365
    float boV2 = 1.0f - (488.0f / 1020.0f); // 0.522

    // Izquierda (oreja izq): x=467-627, y=248-488
    float lU1 = 467.0f / 1320.0f;   // 0.354
    float lU2 = 627.0f / 1320.0f;   // 0.475
    float lV1 = 1.0f - (488.0f / 1020.0f);
    float lV2 = 1.0f - (248.0f / 1020.0f);

    // Derecha (oreja der): x=867-1027, y=248-488
    float rU1 = 867.0f / 1320.0f;   // 0.657
    float rU2 = 1027.0f / 1320.0f;  // 0.778
    float rV1 = 1.0f - (488.0f / 1020.0f);
    float rV2 = 1.0f - (248.0f / 1020.0f);

    // Atras: x=1027-1187, y=248-488
    float bU1 = 1027.0f / 1320.0f;  // 0.778
    float bU2 = 1187.0f / 1320.0f;  // 0.899
    float bV1 = 1.0f - (488.0f / 1020.0f);
    float bV2 = 1.0f - (248.0f / 1020.0f);

    float headVertices[] = {
        // Cara trasera (atras de la cabeza)
        -0.5f, -0.5f, -0.5f,  bU2, bV1,
         0.5f, -0.5f, -0.5f,  bU1, bV1,
         0.5f,  0.5f, -0.5f,  bU1, bV2,
         0.5f,  0.5f, -0.5f,  bU1, bV2,
        -0.5f,  0.5f, -0.5f,  bU2, bV2,
        -0.5f, -0.5f, -0.5f,  bU2, bV1,

        // Cara frontal (cara de Fry con ojos)
        -0.5f, -0.5f,  0.5f,  fU1, fV1,
         0.5f, -0.5f,  0.5f,  fU2, fV1,
         0.5f,  0.5f,  0.5f,  fU2, fV2,
         0.5f,  0.5f,  0.5f,  fU2, fV2,
        -0.5f,  0.5f,  0.5f,  fU1, fV2,
        -0.5f, -0.5f,  0.5f,  fU1, fV1,

        // Cara izquierda
        -0.5f,  0.5f,  0.5f,  lU2, lV2,
        -0.5f,  0.5f, -0.5f,  lU1, lV2,
        -0.5f, -0.5f, -0.5f,  lU1, lV1,
        -0.5f, -0.5f, -0.5f,  lU1, lV1,
        -0.5f, -0.5f,  0.5f,  lU2, lV1,
        -0.5f,  0.5f,  0.5f,  lU2, lV2,

        // Cara derecha
         0.5f,  0.5f,  0.5f,  rU1, rV2,
         0.5f,  0.5f, -0.5f,  rU2, rV2,
         0.5f, -0.5f, -0.5f,  rU2, rV1,
         0.5f, -0.5f, -0.5f,  rU2, rV1,
         0.5f, -0.5f,  0.5f,  rU1, rV1,
         0.5f,  0.5f,  0.5f,  rU1, rV2,

        // Cara inferior (cuello)
        -0.5f, -0.5f, -0.5f,  boU1, boV2,
         0.5f, -0.5f, -0.5f,  boU2, boV2,
         0.5f, -0.5f,  0.5f,  boU2, boV1,
         0.5f, -0.5f,  0.5f,  boU2, boV1,
        -0.5f, -0.5f,  0.5f,  boU1, boV1,
        -0.5f, -0.5f, -0.5f,  boU1, boV2,

        // Cara superior (pelo)
        -0.5f,  0.5f, -0.5f,  tU1, tV1,
         0.5f,  0.5f, -0.5f,  tU2, tV1,
         0.5f,  0.5f,  0.5f,  tU2, tV2,
         0.5f,  0.5f,  0.5f,  tU2, tV2,
        -0.5f,  0.5f,  0.5f,  tU1, tV2,
        -0.5f,  0.5f, -0.5f,  tU1, tV1,
    };

    // VAO/VBO para el piso
    unsigned int floorVAO, floorVBO;
    glGenVertexArrays(1, &floorVAO);
    glGenBuffers(1, &floorVBO);
    glBindVertexArray(floorVAO);
    glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(floorVertices), floorVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // VAO/VBO para la cabeza
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

    // Cargar textura del piso
    unsigned int textureFloor;
    glGenTextures(1, &textureFloor);
    glBindTexture(GL_TEXTURE_2D, textureFloor);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("../practicas/textures/pngwing.com.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load floor texture" << std::endl;
    }
    stbi_image_free(data);

    // Cargar textura de la cabeza
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

    ourShader.use();
    ourShader.setInt("texture1", 0);

    const int FLOOR_SIZE = 10;

    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.5f, 0.7f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourShader.use();

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),
                                                (float)SCR_WIDTH / (float)SCR_HEIGHT,
                                                0.1f, 100.0f);
        ourShader.setMat4("projection", projection);

        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("view", view);

        // Dibujar piso
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureFloor);
        glBindVertexArray(floorVAO);

        for (int x = -FLOOR_SIZE/2; x < FLOOR_SIZE/2; x++)
        {
            for (int z = -FLOOR_SIZE/2; z < FLOOR_SIZE/2; z++)
            {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3((float)x, 0.0f, (float)z));
                ourShader.setMat4("model", model);
                glDrawArrays(GL_TRIANGLES, 0, 30);
            }
        }

        // Dibujar cabeza de Fry
        glBindTexture(GL_TEXTURE_2D, textureHead);
        glBindVertexArray(headVAO);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
        ourShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &floorVAO);
    glDeleteBuffers(1, &floorVBO);
    glDeleteVertexArrays(1, &headVAO);
    glDeleteBuffers(1, &headVBO);

    glfwTerminate();
    return;
}

static void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

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

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}