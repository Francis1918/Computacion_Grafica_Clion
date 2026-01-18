#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include "camera.h"
//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
// ================= CONFIG =================
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

// ================= GEOMETRIA =================
const float ROOM_SIZE = 30.0f;
const float WALL_HEIGHT = 5.0f;
const float HUECO_PASILLO = 4.0f;
const float PASILLO_LENGTH = 20.0f; // longitud del pasillo

// ================= LUZ =================
glm::vec3 lightPos(0.0f, 6.0f, 0.0f);
bool linternaEncendida = false;


// ================= CAMARA =================
static Camera camera(glm::vec3(0.0f, 2.0f, 10.0f));
static float lastX = SCR_WIDTH / 2.0f;
static float lastY = SCR_HEIGHT / 2.0f;
static bool firstMouse = true;

// ================= CALLBACKS =================
static void framebuffer_size_callback(GLFWwindow*, int w, int h) {
    glViewport(0, 0, w, h);
}

static void mouse_callback(GLFWwindow*, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    camera.ProcessMouseMovement(xpos - lastX, lastY - ypos);
    lastX = xpos;
    lastY = ypos;
}

// ================= INPUT =================
bool checkCollision(glm::vec3 pos) {
    float margin = 0.5f; // Margen para que la cámara no se pegue tanto a la pared

    // Límites de la habitación principal
    bool inRoom = (pos.x >= -ROOM_SIZE / 2 + margin && pos.x <= ROOM_SIZE / 2 - margin &&
                   pos.z >= -ROOM_SIZE / 2 + margin && pos.z <= ROOM_SIZE / 2 - margin);

    // Límites del pasillo
    bool inHallway = (pos.x >= -HUECO_PASILLO / 2 + margin && pos.x <= HUECO_PASILLO / 2 - margin &&
                      pos.z >= ROOM_SIZE / 2 - margin && pos.z <= ROOM_SIZE / 2 + PASILLO_LENGTH - margin);

    // El hueco de conexión permite pasar de la habitación al pasillo
    bool inTransition = (pos.x >= -HUECO_PASILLO / 2 + margin && pos.x <= HUECO_PASILLO / 2 - margin &&
                         pos.z >= ROOM_SIZE / 2 - margin && pos.z <= ROOM_SIZE / 2 + margin);

    return !(inRoom || inHallway || inTransition);
}

static void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    glm::vec3 oldPos = camera.Position;
    float velocity = camera.MovementSpeed * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        glm::vec3 nextPos = camera.Position + camera.Front * velocity;
        if (!checkCollision(glm::vec3(nextPos.x, oldPos.y, oldPos.z))) camera.Position.x = nextPos.x;
        if (!checkCollision(glm::vec3(camera.Position.x, oldPos.y, nextPos.z))) camera.Position.z = nextPos.z;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        glm::vec3 nextPos = camera.Position - camera.Front * velocity;
        if (!checkCollision(glm::vec3(nextPos.x, oldPos.y, oldPos.z))) camera.Position.x = nextPos.x;
        if (!checkCollision(glm::vec3(camera.Position.x, oldPos.y, nextPos.z))) camera.Position.z = nextPos.z;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        glm::vec3 nextPos = camera.Position - camera.Right * velocity;
        if (!checkCollision(glm::vec3(nextPos.x, oldPos.y, oldPos.z))) camera.Position.x = nextPos.x;
        if (!checkCollision(glm::vec3(camera.Position.x, oldPos.y, nextPos.z))) camera.Position.z = nextPos.z;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        glm::vec3 nextPos = camera.Position + camera.Right * velocity;
        if (!checkCollision(glm::vec3(nextPos.x, oldPos.y, oldPos.z))) camera.Position.x = nextPos.x;
        if (!checkCollision(glm::vec3(camera.Position.x, oldPos.y, nextPos.z))) camera.Position.z = nextPos.z;
    }
    
    // Forzar altura constante para simular caminar sobre el suelo (opcional, pero común en "hitbox" de FPS)
    camera.Position.y = 2.0f; 

    // Linterna L
    static bool lPrevState = false;
    bool lState = glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS;
    if (lState && !lPrevState) linternaEncendida = !linternaEncendida;
    lPrevState = lState;
}

// ================= TEXTURA =================
static unsigned int loadTexture(const char* path) {
    unsigned int id;
    glGenTextures(1, &id);

    int w, h, c;
    unsigned char* data = stbi_load(path, &w, &h, &c, 0);
    if (data) {
        GLenum format = (c == 4) ? GL_RGBA : GL_RGB;
        glBindTexture(GL_TEXTURE_2D, id);
        glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    stbi_image_free(data);
    return id;
}

// ================= MAIN =================
void game() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Cuarto + Pasillo + Pared Finals", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glEnable(GL_DEPTH_TEST);

    Shader shader("../practicas/shaders/B2T3_game.vs", "../practicas/shaders/B2T3_game.fs");
    Shader lightShader("../practicas/shaders/light_cube_game.vs", "../practicas/shaders/light_cube_game.fs");

    // ===== GEOMETRIA BASE =====
    float planeVertices[] = {
        -0.5f,0,-0.5f,  0,1,0,   0,0,
         0.5f,0,-0.5f,  0,1,0,  10,0,
         0.5f,0, 0.5f,  0,1,0,  10,10,
         0.5f,0, 0.5f,  0,1,0,  10,10,
        -0.5f,0, 0.5f,  0,1,0,   0,10,
        -0.5f,0,-0.5f,  0,1,0,   0,0
    };

    float wallVertices[] = {
        -0.5f,0,0,  0,0,1,  0,1,
         0.5f,0,0,  0,0,1,  4,1,
         0.5f,1,0,  0,0,1,  4,0,
         0.5f,1,0,  0,0,1,  4,0,
        -0.5f,1,0,  0,0,1,  0,0,
        -0.5f,0,0,  0,0,1,  0,1
    };

    float wallFinalVertices[] = { // UVs 0 a 1 para estiramiento
        -0.5f,0,0, 0,0,1, 0.0f,1.0f,
         0.5f,0,0, 0,0,1, 1.0f,1.0f,
         0.5f,1,0, 0,0,1, 1.0f,0.0f,
         0.5f,1,0, 0,0,1, 1.0f,0.0f,
        -0.5f,1,0, 0,0,1, 0.0f,0.0f,
        -0.5f,0,0, 0,0,1, 0.0f,1.0f
    };

    unsigned int floorVAO, floorVBO, wallVAO, wallVBO, wallFinalVAO, wallFinalVBO;
    glGenVertexArrays(1, &floorVAO);
    glGenBuffers(1, &floorVBO);
    glGenVertexArrays(1, &wallVAO);
    glGenBuffers(1, &wallVBO);
    glGenVertexArrays(1, &wallFinalVAO);
    glGenBuffers(1, &wallFinalVBO);

    // piso
    glBindVertexArray(floorVAO);
    glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); glEnableVertexAttribArray(2);

    // paredes normales
    glBindVertexArray(wallVAO);
    glBindBuffer(GL_ARRAY_BUFFER, wallVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(wallVertices), wallVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); glEnableVertexAttribArray(2);

    // pared final
    glBindVertexArray(wallFinalVAO);
    glBindBuffer(GL_ARRAY_BUFFER, wallFinalVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(wallFinalVertices), wallFinalVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); glEnableVertexAttribArray(2);

    // ===== TEXTURAS =====
    unsigned int floorRoomTex = loadTexture("../practicas/textures/TextruaP1.png");
    unsigned int wallRoomTex = loadTexture("../practicas/textures/Pared4.png");
    unsigned int floorHallTex = loadTexture("../practicas/textures/gradas1.png");
    unsigned int wallHallTex = loadTexture("../practicas/textures/PAREDPASILLO3.png");
    unsigned int wallEndTex = loadTexture("../practicas/textures/Finalpa2.png"); // pared final

    // ================= LOOP =================
    while (!glfwWindowShouldClose(window)) {
        float time = glfwGetTime();
        deltaTime = time - lastFrame;
        lastFrame = time;

        processInput(window);

        glClearColor(0.08f, 0.08f, 0.08f, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();

        shader.use();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        shader.setVec3("lightPos", lightPos);
        shader.setVec3("viewPos", camera.Position);

        shader.setInt("linterna", linternaEncendida);
        if (linternaEncendida) {
            shader.setVec3("spotLightPos", camera.Position);
            shader.setVec3("spotLightDir", camera.Front);
            shader.setFloat("spotCutOff", glm::cos(glm::radians(15.0f)));       // corte interno
            shader.setFloat("spotOuterCutOff", glm::cos(glm::radians(25.0f)));  // borde suave
        }
        else {
            shader.setVec3("spotLightPos", glm::vec3(0));
            shader.setVec3("spotLightDir", glm::vec3(0));
            shader.setFloat("spotCutOff", 0.0f);
            shader.setFloat("spotOuterCutOff", 0.0f);
        }


        // ===== PISO CUARTO =====
        glBindTexture(GL_TEXTURE_2D, floorRoomTex);
        glBindVertexArray(floorVAO);
        glm::mat4 model = glm::scale(glm::mat4(1), glm::vec3(ROOM_SIZE));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // ===== PAREDES CUARTO =====
        glBindTexture(GL_TEXTURE_2D, wallRoomTex);
        glBindVertexArray(wallVAO);

        model = glm::translate(glm::mat4(1), glm::vec3(-ROOM_SIZE / 2, 0, 0));
        model = glm::rotate(model, glm::radians(90.f), glm::vec3(0, 1, 0));
        model = glm::scale(model, glm::vec3(ROOM_SIZE, WALL_HEIGHT, 1));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        model = glm::translate(glm::mat4(1), glm::vec3(ROOM_SIZE / 2, 0, 0));
        model = glm::rotate(model, glm::radians(-90.f), glm::vec3(0, 1, 0));
        model = glm::scale(model, glm::vec3(ROOM_SIZE, WALL_HEIGHT, 1));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        model = glm::translate(glm::mat4(1), glm::vec3(0, 0, -ROOM_SIZE / 2));
        model = glm::scale(model, glm::vec3(ROOM_SIZE, WALL_HEIGHT, 1));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        float half = ROOM_SIZE / 2 - HUECO_PASILLO / 2;
        model = glm::translate(glm::mat4(1), glm::vec3(-half / 2 - HUECO_PASILLO / 2, 0, ROOM_SIZE / 2));
        model = glm::scale(model, glm::vec3(half, WALL_HEIGHT, 1));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        model = glm::translate(glm::mat4(1), glm::vec3(half / 2 + HUECO_PASILLO / 2, 0, ROOM_SIZE / 2));
        model = glm::scale(model, glm::vec3(half, WALL_HEIGHT, 1));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // ===== PASILLO =====
        glBindTexture(GL_TEXTURE_2D, floorHallTex);
        glBindVertexArray(floorVAO);
        model = glm::translate(glm::mat4(1), glm::vec3(0, 0, ROOM_SIZE / 2 + 10));
        model = glm::scale(model, glm::vec3(HUECO_PASILLO, 1, PASILLO_LENGTH));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glBindTexture(GL_TEXTURE_2D, wallHallTex);
        glBindVertexArray(wallVAO);

        model = glm::translate(glm::mat4(1), glm::vec3(-HUECO_PASILLO / 2, 0, ROOM_SIZE / 2 + 10));
        model = glm::rotate(model, glm::radians(90.f), glm::vec3(0, 1, 0));
        model = glm::scale(model, glm::vec3(PASILLO_LENGTH, WALL_HEIGHT, 1));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        model = glm::translate(glm::mat4(1), glm::vec3(HUECO_PASILLO / 2, 0, ROOM_SIZE / 2 + 10));
        model = glm::rotate(model, glm::radians(-90.f), glm::vec3(0, 1, 0));
        model = glm::scale(model, glm::vec3(PASILLO_LENGTH, WALL_HEIGHT, 1));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // ===== PARED FINAL =====
        glBindTexture(GL_TEXTURE_2D, wallEndTex);
        glBindVertexArray(wallFinalVAO);
        model = glm::translate(glm::mat4(1), glm::vec3(0, 0, ROOM_SIZE / 2 + PASILLO_LENGTH));
        model = glm::scale(model, glm::vec3(HUECO_PASILLO, WALL_HEIGHT, 1.0f));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // ===== CUBO LUZ =====
        lightShader.use();
        lightShader.setMat4("projection", projection);
        lightShader.setMat4("view", view);
        model = glm::translate(glm::mat4(1), lightPos);
        model = glm::scale(model, glm::vec3(0.4f));
        lightShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return ;//0;
}
