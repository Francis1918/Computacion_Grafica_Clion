cmake_minimum_required(VERSION 3.27)
project(Computacion_Grafica_Clion)
set(CMAKE_CXX_STANDARD 20)
# Agregar el subproyecto de GLFW
add_subdirectory(external/glfw)
# Incluir los headers de GLFW
include_directories(SYSTEM external/glfw/include)
# GLAD
include_directories(SYSTEM external/glad/include)
# GLM
include_directories(SYSTEM external/glm)
# LearnOpenGL (bibliotecas de terceros)
include_directories(SYSTEM external/learnopengl)
add_executable(Computacion_Grafica_Clion main.cpp
        utils.cpp
        practicas/practica1.cpp
        practicas/practica2.cpp
        practicas/practica3.cpp
        external/glad/src/glad.c
        practicas/prueba_opengl.cpp
        practicas/B1T1.cpp
        practicas/FirstTriangle.cpp
        practicas/C2_Exercise_1.cpp
        practicas/C2_Exercise_2.cpp
        practicas/C2_Exercise_3.cpp
        practicas/C2_Exercise_4_Shader_simple.cpp
        practicas/C2_Exercise_5_ShaderColorTime.cpp
        practicas/C2_Exercise_6_ShaderVertexAttri.cpp
        practicas/B1T3.cpp
        practicas/C2_2D_Square_EBO.cpp
        external/learnopengl/shader_s.h
        practicas/C2_Exercise_7_SC_Task3.cpp
        practicas/C2_Exercise_7_SC_Task4.cpp
        practicas/C2_Exercise_7_ShaderClass.cpp
        practicas/B1P2.cpp
        external/learnopengl/stb_image.h
        practicas/C2_Exercise_8_TexureIntro.cpp
        practicas/C2_Exercise_9_TexureUnits.cpp
        practicas/C2_Exercise_9_TU_Task3.cpp
        practicas/C2_Exercise_9_TU_Task4.cpp
        practicas/C2_Exercise_9_TU_Task5.cpp
        practicas/B2T1.cpp
        practicas/C2_Exercise_10_T2D_Task1.cpp
        practicas/C2_Exercise_10_T2D_Task2.cpp
        practicas/C2_Exercise_10_T2D_Task3.cpp
        practicas/C2_Exercise_10_T2D_Task5.cpp
        practicas/C2_Exercise_10_T2D_Task4.cpp
        practicas/C2_Exercise_11_T3D_Task1.cpp
        practicas/C2_Exercise_11_T3D_Task2.cpp
        practicas/C2_Exercise_11_T3D_Task3.cpp
        practicas/C2_Exercise_11_T3D_Task4.cpp)
target_link_libraries(Computacion_Grafica_Clion glfw opengl32)