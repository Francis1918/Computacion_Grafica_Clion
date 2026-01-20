#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/version.h>

void test_assimp() {
    Assimp::Importer importer;
    std::cout << "Assimp Version: " << aiGetVersionMajor() << "." << aiGetVersionMinor() << "." << aiGetVersionRevision() << std::endl;
    
    // Intentar cargar algo inexistente solo para ver si el objeto Importer funciona
    const aiScene* scene = importer.ReadFile("", 0);
    
    if (!scene) {
        std::cout << "Assimp funciona correctamente (pudo intentar cargar un archivo)." << std::endl;
        std::cout << "Error esperado al cargar archivo vacio: " << importer.GetErrorString() << std::endl;
    }
}
