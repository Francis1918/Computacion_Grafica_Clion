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
static unsigned int loadTexture(const char *path);

static const unsigned int SCR_WIDTH = 800;
static const unsigned int SCR_HEIGHT = 600;

static Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
static float lastX = SCR_WIDTH / 2.0f;
static float lastY = SCR_HEIGHT / 2.0f;
static bool firstMouse = true;

static float deltaTime = 0.0f;
static float lastFrame = 0.0f;

static glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

// Variable para seleccionar el tema (1=Desert, 2=Factory, 3=Horror, 4=Biochemical)
static int currentTheme = 1;

void C3_Exercise_15_LCaster_Task5_plus()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Exercise 15 Task 5 Plus - Press 1,2,3,4 to change theme", NULL, NULL);
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

    Shader lightingShader("../practicas/shaders/shader_exercise15t5_casters.vs", "../practicas/shaders/shader_exercise15t5_casters.fs");
    Shader lightCubeShader("../practicas/shaders/shader_exercise15_lightcube.vs", "../practicas/shaders/shader_exercise15_lightcube.fs");

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    glm::vec3 pointLightPositions[] = {
        glm::vec3( 0.7f,  0.2f,  2.0f),
        glm::vec3( 2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3( 0.0f,  0.0f, -3.0f)
    };

    unsigned int VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    unsigned int diffuseMap = loadTexture("../practicas/textures/container2.png");
    unsigned int specularMap = loadTexture("../practicas/textures/container2_specular.png");

    lightingShader.use();
    lightingShader.setInt("material.diffuse", 0);
    lightingShader.setInt("material.specular", 1);

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        // Variables para el tema actual
        glm::vec3 clearColor;
        glm::vec3 pointLightColors[4];
        glm::vec3 dirLightAmbient, dirLightDiffuse, dirLightSpecular;
        glm::vec3 spotLightDiffuse, spotLightSpecular;
        float pointLightLinear, pointLightQuadratic;
        float spotLightLinear, spotLightQuadratic, spotLightCutOff, spotLightOuterCutOff;

        if (currentTheme == 1) // DESERT
        {
            clearColor = glm::vec3(0.75f, 0.52f, 0.3f);
            pointLightColors[0] = glm::vec3(1.0f, 0.6f, 0.0f);
            pointLightColors[1] = glm::vec3(1.0f, 0.0f, 0.0f);
            pointLightColors[2] = glm::vec3(1.0f, 1.0f, 0.0f);
            pointLightColors[3] = glm::vec3(0.2f, 0.2f, 1.0f);
            dirLightAmbient = glm::vec3(0.3f, 0.24f, 0.14f);
            dirLightDiffuse = glm::vec3(0.7f, 0.42f, 0.26f);
            dirLightSpecular = glm::vec3(0.5f, 0.5f, 0.5f);
            spotLightDiffuse = glm::vec3(0.8f, 0.8f, 0.0f);
            spotLightSpecular = glm::vec3(0.8f, 0.8f, 0.0f);
            pointLightLinear = 0.09f; pointLightQuadratic = 0.032f;
            spotLightLinear = 0.09f; spotLightQuadratic = 0.032f;
            spotLightCutOff = 12.5f; spotLightOuterCutOff = 13.0f;
        }
        else if (currentTheme == 2) // FACTORY
        {
            clearColor = glm::vec3(0.1f, 0.1f, 0.1f);
            pointLightColors[0] = glm::vec3(0.2f, 0.2f, 0.6f);
            pointLightColors[1] = glm::vec3(0.3f, 0.3f, 0.7f);
            pointLightColors[2] = glm::vec3(0.0f, 0.0f, 0.3f);
            pointLightColors[3] = glm::vec3(0.4f, 0.4f, 0.4f);
            dirLightAmbient = glm::vec3(0.05f, 0.05f, 0.1f);
            dirLightDiffuse = glm::vec3(0.2f, 0.2f, 0.7f);
            dirLightSpecular = glm::vec3(0.7f, 0.7f, 0.7f);
            spotLightDiffuse = glm::vec3(1.0f, 1.0f, 1.0f);
            spotLightSpecular = glm::vec3(1.0f, 1.0f, 1.0f);
            pointLightLinear = 0.09f; pointLightQuadratic = 0.032f;
            spotLightLinear = 0.009f; spotLightQuadratic = 0.0032f;
            spotLightCutOff = 10.0f; spotLightOuterCutOff = 12.5f;
        }
        else if (currentTheme == 3) // HORROR
        {
            clearColor = glm::vec3(0.0f, 0.0f, 0.0f);
            pointLightColors[0] = glm::vec3(0.1f, 0.1f, 0.1f);
            pointLightColors[1] = glm::vec3(0.1f, 0.1f, 0.1f);
            pointLightColors[2] = glm::vec3(0.1f, 0.1f, 0.1f);
            pointLightColors[3] = glm::vec3(0.3f, 0.1f, 0.1f);
            dirLightAmbient = glm::vec3(0.0f, 0.0f, 0.0f);
            dirLightDiffuse = glm::vec3(0.05f, 0.05f, 0.05f);
            dirLightSpecular = glm::vec3(0.2f, 0.2f, 0.2f);
            spotLightDiffuse = glm::vec3(1.0f, 1.0f, 1.0f);
            spotLightSpecular = glm::vec3(1.0f, 1.0f, 1.0f);
            pointLightLinear = 0.14f; pointLightQuadratic = 0.07f;
            spotLightLinear = 0.09f; spotLightQuadratic = 0.032f;
            spotLightCutOff = 10.0f; spotLightOuterCutOff = 15.0f;
        }
        else // BIOCHEMICAL LAB
        {
            clearColor = glm::vec3(0.9f, 0.9f, 0.9f);
            pointLightColors[0] = glm::vec3(0.4f, 0.7f, 0.1f);
            pointLightColors[1] = glm::vec3(0.4f, 0.7f, 0.1f);
            pointLightColors[2] = glm::vec3(0.4f, 0.7f, 0.1f);
            pointLightColors[3] = glm::vec3(0.4f, 0.7f, 0.1f);
            dirLightAmbient = glm::vec3(0.5f, 0.5f, 0.5f);
            dirLightDiffuse = glm::vec3(1.0f, 1.0f, 1.0f);
            dirLightSpecular = glm::vec3(1.0f, 1.0f, 1.0f);
            spotLightDiffuse = glm::vec3(0.0f, 1.0f, 0.0f);
            spotLightSpecular = glm::vec3(0.0f, 1.0f, 0.0f);
            pointLightLinear = 0.07f; pointLightQuadratic = 0.017f;
            spotLightLinear = 0.07f; spotLightQuadratic = 0.017f;
            spotLightCutOff = 7.0f; spotLightOuterCutOff = 10.0f;
        }

        glClearColor(clearColor.x, clearColor.y, clearColor.z, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        lightingShader.use();
        lightingShader.setVec3("viewPos", camera.Position);
        lightingShader.setFloat("material.shininess", 32.0f);

        // Directional light
        lightingShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        lightingShader.setVec3("dirLight.ambient", dirLightAmbient);
        lightingShader.setVec3("dirLight.diffuse", dirLightDiffuse);
        lightingShader.setVec3("dirLight.specular", dirLightSpecular);

        // Point lights
        for (int i = 0; i < 4; i++)
        {
            std::string idx = std::to_string(i);
            lightingShader.setVec3("pointLights[" + idx + "].position", pointLightPositions[i]);
            lightingShader.setVec3("pointLights[" + idx + "].ambient", pointLightColors[i] * 0.1f);
            lightingShader.setVec3("pointLights[" + idx + "].diffuse", pointLightColors[i]);
            lightingShader.setVec3("pointLights[" + idx + "].specular", pointLightColors[i]);
            lightingShader.setFloat("pointLights[" + idx + "].constant", 1.0f);
            lightingShader.setFloat("pointLights[" + idx + "].linear", pointLightLinear);
            lightingShader.setFloat("pointLights[" + idx + "].quadratic", pointLightQuadratic);
        }

        // SpotLight
        lightingShader.setVec3("spotLight.position", camera.Position);
        lightingShader.setVec3("spotLight.direction", camera.Front);
        lightingShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        lightingShader.setVec3("spotLight.diffuse", spotLightDiffuse);
        lightingShader.setVec3("spotLight.specular", spotLightSpecular);
        lightingShader.setFloat("spotLight.constant", 1.0f);
        lightingShader.setFloat("spotLight.linear", spotLightLinear);
        lightingShader.setFloat("spotLight.quadratic", spotLightQuadratic);
        lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(spotLightCutOff)));
        lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(spotLightOuterCutOff)));

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

        glm::mat4 model = glm::mat4(1.0f);
        lightingShader.setMat4("model", model);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);

        glBindVertexArray(cubeVAO);
        for (unsigned int i = 0; i < 10; i++){
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            lightingShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        lightCubeShader.use();
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);

        glBindVertexArray(lightCubeVAO);
        for (unsigned int i = 0; i < 4; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f));
            lightCubeShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &VBO);

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

    // Cambiar tema con teclas 1, 2, 3, 4
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        currentTheme = 1; // Desert
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        currentTheme = 2; // Factory
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        currentTheme = 3; // Horror
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
        currentTheme = 4; // Biochemical Lab
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

static unsigned int loadTexture(char const * path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}