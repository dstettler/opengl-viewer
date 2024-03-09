#ifndef VAOCONTAINER_H
#define VAOCONTAINER_H

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <glm/vec3.hpp>

#include <GL/glew.h>

struct Face
{
    struct Index
    {
        unsigned int vertex, normal;
    };

    std::vector<Index> indices;
};

class VAOContainer
{
    std::vector<glm::vec3> verts;
    std::vector<glm::vec3> normals;
    std::vector<Face> faces;

    bool wireframe = false;

    unsigned int *vao;
    GLuint *vbo, *ebo;

    std::shared_ptr<float[]> lastVertsArrayGenerated;
    std::shared_ptr<unsigned int[]> lastIndicesArrayGenerated;

    bool isInit = false;

    void readFromFile(std::string filename);

    void init(bool fullDeinit = true);
    void deinit();
    
    public:
    void load(std::string filename, unsigned int* VAO, unsigned int* VBO, unsigned int* EBO);

    std::shared_ptr<float[]> getVertsArray();
    std::shared_ptr<unsigned int[]> getIndicesArray();
    unsigned int getNumVerts();
    
    void scaleMesh(float factor);
    void rotateMesh(float theta);
    void drawGlMesh();
};

#endif // VAOCONTAINER_H