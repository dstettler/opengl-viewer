#ifndef VAOCONTAINER_H
#define VAOCONTAINER_H

#include <memory>
#include <string>
#include <vector>

#include <glm/vec3.hpp>

class VAOContainer
{
    std::vector<glm::vec3> verts;
    std::vector<glm::vec3> normals;

    unsigned int* vao;
    GLuint* vbo;

    std::shared_ptr<float[]> lastVertsArrayGenerated;

    bool isInit = false;

    void readFromFile(std::string filename);

    void deinit();
    
    public:
    void init(std::string filename, unsigned int* VAO, unsigned int* VBO);

    std::shared_ptr<float[]> getVertsArray();
    unsigned int getNumVerts();

    void glSetupMesh();
    void glDrawMesh();
};

#endif // VAOCONTAINER_H