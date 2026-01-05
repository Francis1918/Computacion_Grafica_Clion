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
#include <cmath>

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

void B2T3v2()
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

        // BACK (invertir U)
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

        // RIGHT (invertir U)
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
    // CABEZA FRY (Texture3.png) - UVs corregidos por bordes reales
    // ============================================================
    const float fry_w = 0.42f;
    const float fry_d = 0.42f;
    const float fry_h = fry_w * (169.0f / 242.0f);

    // Texture3.png (W=1320, H=1020), insets 1px
    const float fry_u_back1  = 0.1810606061f;
    const float fry_u_back2  = 0.3628787879f;
    const float fry_u_left1  = 0.3643939394f;
    const float fry_u_left2  = 0.5446969697f;
    const float fry_u_front1 = 0.5462121212f;
    const float fry_u_front2 = 0.7280303030f;
    const float fry_u_right1 = 0.7295454545f;
    const float fry_u_right2 = 0.9113636364f;

    const float fry_v_face_top    = 0.3294117647f;
    const float fry_v_face_bottom = 0.4931372549f;
    const float fry_v_top_top     = 0.0931372549f;
    const float fry_v_top_bottom  = 0.3274509804f;
    const float fry_v_bot_top     = 0.4950980392f;
    const float fry_v_bot_bottom  = 0.7294117647f;

    float headFryVertices[] = {
        // FRONT
        -fry_w, -fry_h,  fry_d,  fry_u_front1, fry_v_face_bottom,
         fry_w, -fry_h,  fry_d,  fry_u_front2, fry_v_face_bottom,
         fry_w,  fry_h,  fry_d,  fry_u_front2, fry_v_face_top,
         fry_w,  fry_h,  fry_d,  fry_u_front2, fry_v_face_top,
        -fry_w,  fry_h,  fry_d,  fry_u_front1, fry_v_face_top,
        -fry_w, -fry_h,  fry_d,  fry_u_front1, fry_v_face_bottom,

        // RIGHT
         fry_w, -fry_h, -fry_d,  fry_u_right2, fry_v_face_bottom,
         fry_w, -fry_h,  fry_d,  fry_u_right1, fry_v_face_bottom,
         fry_w,  fry_h,  fry_d,  fry_u_right1, fry_v_face_top,
         fry_w,  fry_h,  fry_d,  fry_u_right1, fry_v_face_top,
         fry_w,  fry_h, -fry_d,  fry_u_right2, fry_v_face_top,
         fry_w, -fry_h, -fry_d,  fry_u_right2, fry_v_face_bottom,

        // LEFT
        -fry_w, -fry_h, -fry_d,  fry_u_left1,  fry_v_face_bottom,
        -fry_w, -fry_h,  fry_d,  fry_u_left2,  fry_v_face_bottom,
        -fry_w,  fry_h,  fry_d,  fry_u_left2,  fry_v_face_top,
        -fry_w,  fry_h,  fry_d,  fry_u_left2,  fry_v_face_top,
        -fry_w,  fry_h, -fry_d,  fry_u_left1,  fry_v_face_top,
        -fry_w, -fry_h, -fry_d,  fry_u_left1,  fry_v_face_bottom,

        // TOP
        -fry_w,  fry_h, -fry_d,  fry_u_front1, fry_v_top_top,
         fry_w,  fry_h, -fry_d,  fry_u_front2, fry_v_top_top,
         fry_w,  fry_h,  fry_d,  fry_u_front2, fry_v_top_bottom,
         fry_w,  fry_h,  fry_d,  fry_u_front2, fry_v_top_bottom,
        -fry_w,  fry_h,  fry_d,  fry_u_front1, fry_v_top_bottom,
        -fry_w,  fry_h, -fry_d,  fry_u_front1, fry_v_top_top,

        // BOTTOM
        -fry_w, -fry_h, -fry_d,  fry_u_front1, fry_v_bot_bottom,
         fry_w, -fry_h, -fry_d,  fry_u_front2, fry_v_bot_bottom,
         fry_w, -fry_h,  fry_d,  fry_u_front2, fry_v_bot_top,
         fry_w, -fry_h,  fry_d,  fry_u_front2, fry_v_bot_top,
        -fry_w, -fry_h,  fry_d,  fry_u_front1, fry_v_bot_top,
        -fry_w, -fry_h, -fry_d,  fry_u_front1, fry_v_bot_bottom,

        // BACK (invertir U)
        -fry_w, -fry_h, -fry_d,  fry_u_back2,  fry_v_face_bottom,
         fry_w, -fry_h, -fry_d,  fry_u_back1,  fry_v_face_bottom,
         fry_w,  fry_h, -fry_d,  fry_u_back1,  fry_v_face_top,
         fry_w,  fry_h, -fry_d,  fry_u_back1,  fry_v_face_top,
        -fry_w,  fry_h, -fry_d,  fry_u_back2,  fry_v_face_top,
        -fry_w, -fry_h, -fry_d,  fry_u_back2,  fry_v_face_bottom,
    };

    // ============================================================
    // CABEZA GARFIELD (Garfield.jpg)
    // Orden de lados: BACK | LEFT | FRONT (cara) | RIGHT
    // Garfield.jpg (W=1490, H=1177), insets 1px
    // ============================================================
    const float gar_w = 0.42f;
    const float gar_d = 0.42f;

    const float gar_u_back1  = 0.1469798658f;
    const float gar_u_back2  = 0.3483221477f;
    const float gar_u_left1  = 0.3496644295f;
    const float gar_u_left2  = 0.5510067114f;
    const float gar_u_front1 = 0.5523489933f;
    const float gar_u_front2 = 0.7530201342f;
    const float gar_u_right1 = 0.7543624161f;
    const float gar_u_right2 = 0.9550335570f;

    const float gar_v_side_top    = 0.3160577740f;
    const float gar_v_side_bottom = 0.4919286321f;

    const float gar_v_top_top     = 0.0594732370f;
    const float gar_v_top_bottom  = 0.3143585387f;

    const float gar_h = gar_w * (207.0f / 299.0f);

    float headGarVertices[] = {
        // FRONT (cara)
        -gar_w, -gar_h,  gar_d,  gar_u_front1, gar_v_side_bottom,
         gar_w, -gar_h,  gar_d,  gar_u_front2, gar_v_side_bottom,
         gar_w,  gar_h,  gar_d,  gar_u_front2, gar_v_side_top,
         gar_w,  gar_h,  gar_d,  gar_u_front2, gar_v_side_top,
        -gar_w,  gar_h,  gar_d,  gar_u_front1, gar_v_side_top,
        -gar_w, -gar_h,  gar_d,  gar_u_front1, gar_v_side_bottom,

        // RIGHT
         gar_w, -gar_h, -gar_d,  gar_u_right2, gar_v_side_bottom,
         gar_w, -gar_h,  gar_d,  gar_u_right1, gar_v_side_bottom,
         gar_w,  gar_h,  gar_d,  gar_u_right1, gar_v_side_top,
         gar_w,  gar_h,  gar_d,  gar_u_right1, gar_v_side_top,
         gar_w,  gar_h, -gar_d,  gar_u_right2, gar_v_side_top,
         gar_w, -gar_h, -gar_d,  gar_u_right2, gar_v_side_bottom,

        // LEFT
        -gar_w, -gar_h, -gar_d,  gar_u_left1,  gar_v_side_bottom,
        -gar_w, -gar_h,  gar_d,  gar_u_left2,  gar_v_side_bottom,
        -gar_w,  gar_h,  gar_d,  gar_u_left2,  gar_v_side_top,
        -gar_w,  gar_h,  gar_d,  gar_u_left2,  gar_v_side_top,
        -gar_w,  gar_h, -gar_d,  gar_u_left1,  gar_v_side_top,
        -gar_w, -gar_h, -gar_d,  gar_u_left1,  gar_v_side_bottom,

        // TOP
        -gar_w,  gar_h, -gar_d,  gar_u_front1, gar_v_top_top,
         gar_w,  gar_h, -gar_d,  gar_u_front2, gar_v_top_top,
         gar_w,  gar_h,  gar_d,  gar_u_front2, gar_v_top_bottom,
         gar_w,  gar_h,  gar_d,  gar_u_front2, gar_v_top_bottom,
        -gar_w,  gar_h,  gar_d,  gar_u_front1, gar_v_top_bottom,
        -gar_w,  gar_h, -gar_d,  gar_u_front1, gar_v_top_top,

        // BOTTOM (reutiliza TOP por ahora)
        -gar_w, -gar_h, -gar_d,  gar_u_front1, gar_v_top_bottom,
         gar_w, -gar_h, -gar_d,  gar_u_front2, gar_v_top_bottom,
         gar_w, -gar_h,  gar_d,  gar_u_front2, gar_v_top_top,
         gar_w, -gar_h,  gar_d,  gar_u_front2, gar_v_top_top,
        -gar_w, -gar_h,  gar_d,  gar_u_front1, gar_v_top_top,
        -gar_w, -gar_h, -gar_d,  gar_u_front1, gar_v_top_bottom,

        // BACK (invertir U)
        -gar_w, -gar_h, -gar_d,  gar_u_back2,  gar_v_side_bottom,
         gar_w, -gar_h, -gar_d,  gar_u_back1,  gar_v_side_bottom,
         gar_w,  gar_h, -gar_d,  gar_u_back1,  gar_v_side_top,
         gar_w,  gar_h, -gar_d,  gar_u_back1,  gar_v_side_top,
        -gar_w,  gar_h, -gar_d,  gar_u_back2,  gar_v_side_top,
        -gar_w, -gar_h, -gar_d,  gar_u_back2,  gar_v_side_bottom,
    };

    // ============================================================
    // VAOs/VBOs
    // ============================================================
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

    unsigned int headFryVAO, headFryVBO;
    glGenVertexArrays(1, &headFryVAO);
    glGenBuffers(1, &headFryVBO);
    glBindVertexArray(headFryVAO);
    glBindBuffer(GL_ARRAY_BUFFER, headFryVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(headFryVertices), headFryVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int headGarVAO, headGarVBO;
    glGenVertexArrays(1, &headGarVAO);
    glGenBuffers(1, &headGarVBO);
    glBindVertexArray(headGarVAO);
    glBindBuffer(GL_ARRAY_BUFFER, headGarVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(headGarVertices), headGarVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // ============================================================
    // Texturas
    // ============================================================
    unsigned int textureFloor   = LoadTexture2D("../practicas/textures/pngwing.com.png", false, true, true, false);
    unsigned int textureFryHead = LoadTexture2D("../practicas/textures/Texture3.png",     false, true, true, false);
    unsigned int textureGarHead = LoadTexture2D("../practicas/textures/Garfield.jpg",     false, true, true, false);

    ourShader.use();
    ourShader.setInt("texture1", 0);

    const int FLOOR_SIZE = 10;

    // ============================================================
    // POSICIONES BASE (duplicados)
    // Cada cabeza hace:
    // - Giro en su propio lugar (spin)
    // - Movimiento elíptico alrededor de su posición base (orbit)
    // Si quieres solo spin: pon orbitA=0 y orbitB=0.
    // ============================================================
    const float gap = 0.10f;
    const float xGar = (fry_w + gar_w) + gap;

    const glm::vec3 fryBase1(0.00f, fry_h, 0.00f);
    const glm::vec3 fryBase2(0.00f, fry_h, 1.40f);

    const glm::vec3 garBase1(xGar, gar_h, 0.00f);
    const glm::vec3 garBase2(xGar, gar_h, 1.40f);

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = (float)glfwGetTime();
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

        // ============================
        // Animación
        // ============================
        const float t = currentFrame;

        auto drawHead = [&](unsigned int vao, unsigned int tex, const glm::vec3& base,
                            float orbitA, float orbitB, float orbitSpeed, float phase,
                            float spinSpeed, float spinTilt)
        {
            // Elipse en XZ alrededor de la base
            glm::vec3 pos = base + glm::vec3(
                orbitA * std::cos(orbitSpeed * t + phase),
                0.0f,
                orbitB * std::sin(orbitSpeed * t + phase)
            );

            // Giro en su lugar
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, pos);
            model = glm::rotate(model, spinTilt, glm::vec3(1.0f, 0.0f, 0.0f));         // leve inclinación opcional
            model = glm::rotate(model, spinSpeed * t, glm::vec3(0.0f, 1.0f, 0.0f));    // spin Y

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, tex);
            glBindVertexArray(vao);
            ourShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        };

        // ============================
        // Duplicados FRY (2)
        // ============================
        drawHead(headFryVAO, textureFryHead, fryBase1,
                 0.25f, 0.15f, 1.20f, 0.00f,     // órbita
                 1.80f, 0.05f);                  // spin + tilt

        drawHead(headFryVAO, textureFryHead, fryBase2,
                 0.18f, 0.12f, 1.50f, 1.57f,
                 2.20f, -0.04f);

        // ============================
        // Duplicados GARFIELD (2)
        // ============================
        drawHead(headGarVAO, textureGarHead, garBase1,
                 0.25f, 0.15f, 1.10f, 0.70f,
                 1.60f, 0.04f);

        drawHead(headGarVAO, textureGarHead, garBase2,
                 0.18f, 0.12f, 1.45f, 2.20f,
                 2.00f, -0.05f);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &floorVAO);
    glDeleteBuffers(1, &floorVBO);

    glDeleteVertexArrays(1, &headFryVAO);
    glDeleteBuffers(1, &headFryVBO);

    glDeleteVertexArrays(1, &headGarVAO);
    glDeleteBuffers(1, &headGarVBO);

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
    (void)window;
    glViewport(0, 0, width, height);
}

static void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    (void)window;

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
    (void)window;
    (void)xoffset;
    camera.ProcessMouseScroll((float)yoffset);
}
