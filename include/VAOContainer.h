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
    struct Line
    {
        float p1, p2;
    };

    std::vector<Line> lines;
};

class VAOContainer
{
    std::vector<glm::vec3> verts;
    std::vector<glm::vec3> normals;
    std::vector<Face> faces;

    bool wireframe = true;

    unsigned int* vao;
    GLuint* vbo;

    std::shared_ptr<float[]> lastVertsArrayGenerated;

    bool isInit = false;

    void readFromFile(std::string filename);

    void init(bool fullDeinit = true);
    void deinit();
    
    public:
    void load(std::string filename, unsigned int* VAO, unsigned int* VBO);

    std::shared_ptr<float[]> getVertsArray();
    unsigned int getNumVerts();
    
    void scaleMesh(float factor);
    void rotateMesh(float theta);
    void drawGlMesh();
};

#endif // VAOCONTAINER_H