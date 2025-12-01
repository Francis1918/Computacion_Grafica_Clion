#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void prueba_opengl() {

    // Inicializar GLFW
    if (!glfwInit()) {
        std::cout << "Error inicializando GLFW" << std::endl;
        return;
    }

    // Configurar la versión de OpenGL (3.3 Core)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Crear ventana
    GLFWwindow* window = glfwCreateWindow(800, 600, "Prueba OpenGL", nullptr, nullptr);
    if (!window) {
        std::cout << "No se pudo crear la ventana GLFW" << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);

    // Inicializar GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Error inicializando GLAD" << std::endl;
        glfwTerminate();
        return;
    }

    // ====== PRUEBA GLM ======
    glm::vec3 v(1.0f, 2.0f, 3.0f);
    std::cout << "GLM funcionando: "
              << v.x << ", " << v.y << ", " << v.z << std::endl;

    // Ejemplo extra: matriz de transformación
    glm::mat4 identidad = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(identidad, glm::vec3(1.0f, 0.0f, 0.0f));
    (void)translate; // solo para que no dé warning de variable no usada
    // ========================

    // Viewport
    glViewport(0, 0, 800, 600);

    // Bucle principal
    while (!glfwWindowShouldClose(window)) {
        // Color de fondo
        glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}
