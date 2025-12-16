#include <iostream>
/*
void practica1();
void practica2();
void practica3();
void prueba_opengl();
solo por depuracion del proyecto
*/
void B1T1();
void FirstTraingle();
void C2_2D_Square_EBO();
void C2_Exercise_1();
void C2_Exercise_2();
void C2_Exercise_3();
void C2_Exercise_4_Shader_simple();
void C2_Exercise_5_ShaderColorTime();
void C2_Exercise_6_ShaderVertexAttri();
void B1T3();
void utils();
void C2_Exercise_7_SC_Task3();
void C2_Exercise_7_SC_Task4();
void C2_Exercise_7_ShaderClass();
void B1P2();
void C2_Exercise_8_TexureIntro();
void C2_Exercise_9_TexureUnits();
void C2_Exercise_9_TU_Task3();
void C2_Exercise_9_TU_Task4();
void C2_Exercise_9_TU_Task5();
void B2T1();
void C2_Exercise_10_T2D_Task1();
void C2_Exercise_10_T2D_Task2();
void C2_Exercise_10_T2D_Task3();
void C2_Exercise_10_T2D_Task4();
void C2_Exercise_10_T2D_Task5();
void C2_Exercise_11_T3D_Task1();
void C2_Exercise_11_T3D_Task2();
void C2_Exercise_11_T3D_Task3();
void C2_Exercise_11_T3D_Task4();
void B2T2();
void C2_Exercise_12_VP3D_Task1();
void C2_Exercise_12_VP3D_Task2();
void C2_Exercise_12_VP3D_Task3();
void C2_Exercise_12_VP3D_Task4();
int main() {
    int opcion;
    // Mostrar menú detallado para que el usuario sepa qué está eligiendo
    std::cout << "===== Proyecto Computacion Grafica =====" << std::endl;
    std::cout << "1. B1T1 - Ventana inicial" << std::endl;
    std::cout << "2. FirstTriangle - Primer triangulo" << std::endl;
    std::cout << "3. C2_2D_Square_EBO - Cuadrado con EBO" << std::endl;
    std::cout << "4. C2_Exercise_1 - Dos triangulos" << std::endl;
    std::cout << "5. C2_Exercise_2 - Dos triangulos con dos VAOs" << std::endl;
    std::cout << "6. C2_Exercise_3 - Dos triangulos con dos fragment shaders" << std::endl;
    std::cout << "7. C2_Exercise_4_Shader_simple - Shader simple" << std::endl;
    std::cout << "8. C2_Exercise_5_ShaderColorTime - Shader con color tiempo" << std::endl;
    std::cout << "9. C2_Exercise_6_ShaderVertexAttri - Atributos de vertice" << std::endl;
    std::cout << "10. B1T3 - Octagono con colores" << std::endl;
    std::cout << "11. C2_Exercise_7_SC_Task3 - Shader con offset" << std::endl;
    std::cout << "12. C2_Exercise_7_SC_Task4 - Shader con animacion" << std::endl;
    std::cout << "13. C2_Exercise_7_ShaderClass - Shader usando clase Shader" << std::endl;
    std::cout << "14. B1P2 - Figura 3 con shaders y animacion" << std::endl;
    std::cout << "15. C2_Exercise_8_TexureIntro - Introduccion a texturas" << std::endl;
    std::cout << "16. C2_Exercise_9_TexureUnits - Unidades de textura" << std::endl;
    std::cout << "17. C2_Exercise_9_TU_Task3 - Textura con coordenadas mayores a 1" << std::endl;
    std::cout << "18. C2_Exercise_9_TU_Task4 - Textura con diferentes parametros de wrapping y filtering" << std::endl;
    std::cout << "19. C2_Exercise_9_TU_Task5 - Textura con parametros avanzados" << std::endl;
    std::cout << "20. B2T1 - Texturas" << std::endl;
    std::cout << "21. C2_Exercise_10_T2D_Task1 - Transformaciones 2D" << std::endl;
    std::cout << "22. C2_Exercise_10_T2D_Task2 - Transformaciones 2D con matriz" << std::endl;
    std::cout << "23. C2_Exercise_10_T2D_Task3 - Transformaciones 2D con animacion" << std::endl;
    std::cout << "24. C2_Exercise_10_T2D_Task4 - Transformaciones 2D con animacion 2" << std::endl;
    std::cout << "25. C2_Exercise_10_T2D_Task5 - Transformaciones 2D con parametros avanzados" << std::endl;
    std::cout << "26. C2_Exercise_11_T3D_Task1 - Transformaciones 3D con texturas" << std::endl;
    std::cout << "27. C2_Exercise_11_T3D_Task2 - Transformaciones 3D con texturas y animacion" << std::endl;
    std::cout << "28. C2_Exercise_11_T3D_Task3 - Transformaciones 3D con texturas y animacion avanzada" << std::endl;
    std::cout << "29. C2_Exercise_11_T3D_Task4 - Transformaciones 3D con texturas y animacion avanzada con profundidad" << std::endl;
    std::cout << "30. B2T2 - Texturas con multiples atributos" << std::endl;
    std::cout << "31. C2_Exercise_12_VP3D_Task1 - Vista de camara 3D con animacion" << std::endl;
    std::cout << "32. C2_Exercise_12_VP3D_Task2 - Vista de camara 3D con control de usuario" << std::endl;
    std::cout << "33. C2_Exercise_12_VP3D_Task3 - Vista de camara 3D con control de usuario avanzado" << std::endl;
    std::cout << "34. C2_Exercise_12_VP3D_Task4 - Vista de camara 3D con control de usuario y profundidad" << std::endl;
    std::cout << "0. Salir" << std::endl;
    std::cout << "Elige la practica que deseas ejecutar: ";
    std::cin >> opcion;
    switch (opcion) {
        case 1:
            B1T1();//ventana
            break;
        case 2:
            FirstTraingle();//triangulo
            break;
        case 3:
            C2_2D_Square_EBO();//cuadrado con EBO
            break;
        case 4:
            C2_Exercise_1();//dos triangulos
            break;
        case 5:
            C2_Exercise_2();//dos triangulos con dos VAOs
            break;
        case 6:
            C2_Exercise_3();//dos triangulos con dos fragment shaders
            break;
        case 7:
            C2_Exercise_4_Shader_simple();//shader simple
            break;
        case 8:
            C2_Exercise_5_ShaderColorTime();//shader color tiempo
            break;
        case 9:
            C2_Exercise_6_ShaderVertexAttri();//shader con atributos de vertice
            break;
        case 10:
            B1T3();//octogono con colores
            break;
        case 11:
            C2_Exercise_7_SC_Task3();//shader con offset
            break;
        case 12:
            C2_Exercise_7_SC_Task4();//shader con animacion
            break;
        case 13:
            C2_Exercise_7_ShaderClass();//shader usando clase Shader
            break;
        case 14:
            B1P2();//figura 3 con shaders y animacion
            break;
        case 15:
            C2_Exercise_8_TexureIntro();
            break;
        case 16:
            C2_Exercise_9_TexureUnits();//unidades de textura
            break;
        case 17:
            C2_Exercise_9_TU_Task3();//textura con coordenadas mayores a 1
            break;
        case 18:
            C2_Exercise_9_TU_Task4();//textura con diferentes parametros de wrapping y filtering
            break;
        case 19:
            C2_Exercise_9_TU_Task5();//textura con parametros avanzados
            break;
        case 20:
            B2T1();//texturas del codigo ya actualizado (Deber)
            break;
        case 21:
            C2_Exercise_10_T2D_Task1();//transformaciones 2D
            break;
        case 22:
            C2_Exercise_10_T2D_Task2();//transformaciones 2D con matriz
            break;
        case 23:
            C2_Exercise_10_T2D_Task3();//transformaciones 2D con animacion
            break;
        case 24:
            C2_Exercise_10_T2D_Task4();//transformaciones 2D con animacion 2
            break;
        case 25:
            C2_Exercise_10_T2D_Task5();//transformaciones 2D con parametros avanzados
            break;
        case 26:
            C2_Exercise_11_T3D_Task1();//transformaciones 3D con texturas
            break;
        case 27:
            C2_Exercise_11_T3D_Task2();//transformaciones 3D con texturas y animacion
            break;
        case 28:
            C2_Exercise_11_T3D_Task3();//transformaciones 3D con texturas y animacion avanzada
            break;
        case 29:
            C2_Exercise_11_T3D_Task4();//transformaciones 3D con texturas y animacion avanzada con profundidad
            break;
        case 30:
            B2T2();//texturas con multiples atributos
            break;
        case 32:
            C2_Exercise_12_VP3D_Task2();//vista de camara 3D con control de usuario
            break;
        case 31:
            C2_Exercise_12_VP3D_Task1();
            break;
        case 33:
            C2_Exercise_12_VP3D_Task3();//vista de camara
            break;
        case 34:
            C2_Exercise_12_VP3D_Task4();//vista de camara 3D con control de usuario y profundidad
            break;
        case 0:
            std::cout << "Saliendo..." << std::endl;
            break;
        default:
            std::cout << "Opcion no valida" << std::endl;
            break;
    }
    return 0;
}