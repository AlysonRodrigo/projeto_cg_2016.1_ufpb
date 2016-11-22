#ifndef LOADER_H
#define LOADER_H

#include <iostream>
#include <vector>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <string>
// Header file
#include <glm/fwd.hpp>
// Source file
#include <glm/glm.hpp>
#include <glm/ext.hpp>


class Loader
{
public:
    Loader();
    ~Loader();
    void loadScene(const char* filePath);
    void loadMeshes(const aiScene* loadedScene);
    void loadNormals(aiMesh* loadedMesh);
    void loadVertices(aiMesh* loadedMesh);
    void loadTextures(aiMesh* loadedMesh);
    void PrintVec3(glm::vec3 v3);
    void PrintVec2(glm::vec2 v2);
    void PrintVector(std::vector <glm::vec3> vec);


//private:
    std::vector <glm::vec3> vertexArray;
    std::vector <glm::vec3> normalArray;
    std::vector <glm::vec2> textureArray;
    int numVertices;
    const aiScene* myScene;

};

#endif
