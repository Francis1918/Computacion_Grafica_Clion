//
// Francis Bravo 1726296815
// Escena tipo Minecraft con piso y personaje
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
static void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

static Camera camera(glm::vec3(0.0f, 2.0f, 8.0f));
static float lastX = SCR_WIDTH / 2.0f;
static float lastY = SCR_HEIGHT / 2.0f;
static bool firstMouse = true;

static float deltaTime = 0.0f;
static float lastFrame = 0.0f;

static unsigned int LoadTexture2D(const char* path, bool flipY, bool clampToEdge, bool nearest, bool genMipmaps)
{
    unsigned int texID = 0;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, clampToEdge ? GL_CLAMP_TO_EDGE : GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, clampToEdge ? GL_CLAMP_TO_EDGE : GL_REPEAT);

    if (nearest)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, genMipmaps ? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, genMipmaps ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    int width = 0, height = 0, nrChannels = 0;
    stbi_set_flip_vertically_on_load(flipY);
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (!data)
    {
        std::cout << "Failed to load texture: " << path << std::endl;
        return texID;
    }

    GLenum format = GL_RGB;
    if (nrChannels == 1) format = GL_RED;
    else if (nrChannels == 3) format = GL_RGB;
    else if (nrChannels == 4) format = GL_RGBA;

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, (GLint)format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    if (genMipmaps) glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
    return texID;
}

void B2T3()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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
    // PISO: uv recortados del net (pngwing.com.png), ignorando pestañas negras
    // ============================================================
    float topU1 = 0.51349528f, topU2 = 0.71187584f, topV1 = 0.63512361f, topV2 = 0.88576300f;
    float leftU1 = 0.10391363f, leftU2 = 0.30229420f, leftV1 = 0.37595908f, leftV2 = 0.62745098f;
    float backU1 = 0.30904184f, backU2 = 0.50674764f, backV1 = 0.37595908f, backV2 = 0.62745098f;
    float frontU1 = 0.51349528f, frontU2 = 0.71187584f, frontV1 = 0.37595908f, frontV2 = 0.62745098f;
    float rightU1 = 0.71862348f, rightU2 = 0.91632928f, rightV1 = 0.37595908f, rightV2 = 0.62745098f;
    float bottomU1 = 0.51349528f, bottomU2 = 0.71187584f, bottomV1 = 0.11679454f, bottomV2 = 0.36743393f;

    float floorVertices[] = {
        // TOP
        -0.5f, 0.0f, -0.5f,   topU1, topV1,
         0.5f, 0.0f, -0.5f,   topU2, topV1,
         0.5f, 0.0f,  0.5f,   topU2, topV2,
         0.5f, 0.0f,  0.5f,   topU2, topV2,
        -0.5f, 0.0f,  0.5f,   topU1, topV2,
        -0.5f, 0.0f, -0.5f,   topU1, topV1,

        // FRONT
        -0.5f, -0.5f,  0.5f,  frontU1, frontV1,
         0.5f, -0.5f,  0.5f,  frontU2, frontV1,
         0.5f,  0.0f,  0.5f,  frontU2, frontV2,
         0.5f,  0.0f,  0.5f,  frontU2, frontV2,
        -0.5f,  0.0f,  0.5f,  frontU1, frontV2,
        -0.5f, -0.5f,  0.5f,  frontU1, frontV1,

        // BACK (invertir U para no espejo)
        -0.5f, -0.5f, -0.5f,  backU2, backV1,
         0.5f, -0.5f, -0.5f,  backU1, backV1,
         0.5f,  0.0f, -0.5f,  backU1, backV2,
         0.5f,  0.0f, -0.5f,  backU1, backV2,
        -0.5f,  0.0f, -0.5f,  backU2, backV2,
        -0.5f, -0.5f, -0.5f,  backU2, backV1,

        // LEFT
        -0.5f, -0.5f, -0.5f,  leftU1, leftV1,
        -0.5f, -0.5f,  0.5f,  leftU2, leftV1,
        -0.5f,  0.0f,  0.5f,  leftU2, leftV2,
        -0.5f,  0.0f,  0.5f,  leftU2, leftV2,
        -0.5f,  0.0f, -0.5f,  leftU1, leftV2,
        -0.5f, -0.5f, -0.5f,  leftU1, leftV1,

        // RIGHT (invertir U para no espejo)
         0.5f, -0.5f, -0.5f,  rightU2, rightV1,
         0.5f, -0.5f,  0.5f,  rightU1, rightV1,
         0.5f,  0.0f,  0.5f,  rightU1, rightV2,
         0.5f,  0.0f,  0.5f,  rightU1, rightV2,
         0.5f,  0.0f, -0.5f,  rightU2, rightV2,
         0.5f, -0.5f, -0.5f,  rightU2, rightV1,

        // BOTTOM
        -0.5f, -0.5f, -0.5f,  bottomU1, bottomV1,
         0.5f, -0.5f, -0.5f,  bottomU2, bottomV1,
         0.5f, -0.5f,  0.5f,  bottomU2, bottomV2,
         0.5f, -0.5f,  0.5f,  bottomU2, bottomV2,
        -0.5f, -0.5f,  0.5f,  bottomU1, bottomV2,
        -0.5f, -0.5f, -0.5f,  bottomU1, bottomV1,
    };

    // ============================================================
    // CABEZA DE FRY (Texture3.png) - UVs corregidos por bordes reales
    //
    // Importante:
    // - stbi flipY = false, así que V=0 es ARRIBA y V=1 es ABAJO.
    // - Se usan insets de 1px para evitar bleeding.
    // ============================================================

    // Dimensiones: ajustar altura para que no estire la cara (rectángulo 242x169 aprox)
    const float h_w = 0.42f;                          // half width
    const float h_d = 0.42f;                          // half depth
    const float h_h = h_w * (169.0f / 242.0f);        // half height (match panel aspect)

    // U boundaries (inset 1px), W=1320
    const float u_back1  = 0.1810606061f; // (238+1)/1320
    const float u_back2  = 0.3628787879f; // (480-1)/1320
    const float u_left1  = 0.3643939394f; // (480+1)/1320
    const float u_left2  = 0.5446969697f; // (720-1)/1320
    const float u_front1 = 0.5462121212f; // (720+1)/1320
    const float u_front2 = 0.7280303030f; // (962-1)/1320
    const float u_right1 = 0.7295454545f; // (962+1)/1320
    const float u_right2 = 0.9113636364f; // (1204-1)/1320

    // V boundaries (inset 1px), H=1020
    // Franja de la cara (donde están ojos) ~ y:[335..504]
    const float v_face_top    = 0.3294117647f; // (335+1)/1020
    const float v_face_bottom = 0.4931372549f; // (504-1)/1020

    // Top (cabello arriba del panel frontal) ~ y:[94..335]
    const float v_top_top     = 0.0931372549f; // (94+1)/1020
    const float v_top_bottom  = 0.3274509804f; // (335-1)/1020

    // Bottom (zona debajo de la franja) ~ y:[504..745] (no suele verse)
    const float v_bot_top     = 0.4950980392f; // (504+1)/1020
    const float v_bot_bottom  = 0.7294117647f; // (745-1)/1020

    float headVertices[] = {
        // --- FRONT (Z+) ---
        -h_w, -h_h,  h_d,  u_front1, v_face_bottom,
         h_w, -h_h,  h_d,  u_front2, v_face_bottom,
         h_w,  h_h,  h_d,  u_front2, v_face_top,
         h_w,  h_h,  h_d,  u_front2, v_face_top,
        -h_w,  h_h,  h_d,  u_front1, v_face_top,
        -h_w, -h_h,  h_d,  u_front1, v_face_bottom,

        // --- RIGHT (X+) ---
         h_w, -h_h, -h_d,  u_right2, v_face_bottom, // back
         h_w, -h_h,  h_d,  u_right1, v_face_bottom, // front (seam with front)
         h_w,  h_h,  h_d,  u_right1, v_face_top,
         h_w,  h_h,  h_d,  u_right1, v_face_top,
         h_w,  h_h, -h_d,  u_right2, v_face_top,    // back
         h_w, -h_h, -h_d,  u_right2, v_face_bottom,

        // --- LEFT (X-) ---
        -h_w, -h_h, -h_d,  u_left1,  v_face_bottom, // back
        -h_w, -h_h,  h_d,  u_left2,  v_face_bottom, // front (seam with front)
        -h_w,  h_h,  h_d,  u_left2,  v_face_top,
        -h_w,  h_h,  h_d,  u_left2,  v_face_top,
        -h_w,  h_h, -h_d,  u_left1,  v_face_top,    // back
        -h_w, -h_h, -h_d,  u_left1,  v_face_bottom,

        // --- TOP (Y+) ---
        // z=-h_d usa v_top_top (más arriba), z=+h_d usa v_top_bottom (borde con la cara)
        -h_w,  h_h, -h_d,  u_front1, v_top_top,
         h_w,  h_h, -h_d,  u_front2, v_top_top,
         h_w,  h_h,  h_d,  u_front2, v_top_bottom,
         h_w,  h_h,  h_d,  u_front2, v_top_bottom,
        -h_w,  h_h,  h_d,  u_front1, v_top_bottom,
        -h_w,  h_h, -h_d,  u_front1, v_top_top,

        // --- BOTTOM (Y-) ---
        // z=+h_d usa v_bot_top (borde con la cara), z=-h_d usa v_bot_bottom
        -h_w, -h_h, -h_d,  u_front1, v_bot_bottom,
         h_w, -h_h, -h_d,  u_front2, v_bot_bottom,
         h_w, -h_h,  h_d,  u_front2, v_bot_top,
         h_w, -h_h,  h_d,  u_front2, v_bot_top,
        -h_w, -h_h,  h_d,  u_front1, v_bot_top,
        -h_w, -h_h, -h_d,  u_front1, v_bot_bottom,

        // --- BACK (Z-) ---
        // invertir U para que no quede espejado y para continuidad con el net
        -h_w, -h_h, -h_d,  u_back2,  v_face_bottom, // left edge uses right edge of back panel
         h_w, -h_h, -h_d,  u_back1,  v_face_bottom,
         h_w,  h_h, -h_d,  u_back1,  v_face_top,
         h_w,  h_h, -h_d,  u_back1,  v_face_top,
        -h_w,  h_h, -h_d,  u_back2,  v_face_top,
        -h_w, -h_h, -h_d,  u_back2,  v_face_bottom,
    };

    // VAO/VBO piso
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

    // VAO/VBO cabeza
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

    // Texturas
    // Piso: clamp + nearest + sin mipmaps
    unsigned int textureFloor = LoadTexture2D("../practicas/textures/pngwing.com.png", false, true, true, false);

    // Cabeza: clamp + nearest + sin mipmaps (crisp, sin bleeding)
    unsigned int textureHead  = LoadTexture2D("../practicas/textures/Texture3.png", false, true, true, false);

    ourShader.use();
    ourShader.setInt("texture1", 0);

    const int FLOOR_SIZE = 10;

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
            (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        ourShader.setMat4("projection", projection);

        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("view", view);

        // Piso
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureFloor);
        glBindVertexArray(floorVAO);

        for (int x = -FLOOR_SIZE / 2; x < FLOOR_SIZE / 2; x++)
        {
            for (int z = -FLOOR_SIZE / 2; z < FLOOR_SIZE / 2; z++)
            {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3((float)x, 0.0f, (float)z));
                ourShader.setMat4("model", model);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
        }

        // Cabeza
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureHead);
        glBindVertexArray(headVAO);

        glm::mat4 model = glm::mat4(1.0f);
        // colocar la base de la cabeza justo sobre el piso: translate Y = h_h
        model = glm::translate(model, glm::vec3(0.0f, h_h, 0.0f));
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

static void processInput(GLFWwindow* window)
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
        lastX = (float)xpos;
        lastY = (float)ypos;
        firstMouse = false;
    }

    float xoffset = (float)xpos - lastX;
    float yoffset = lastY - (float)ypos;
    lastX = (float)xpos;
    lastY = (float)ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    (void)xoffset;
    camera.ProcessMouseScroll((float)yoffset);
}
