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
    public:
    enum class MeshRotation
    {
        XAxis,
        YAxis,
        ZAxis,
    };

    private:
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
    
    bool isAlreadyTri(Face *f);
    std::vector<Face> triangulateFace(Face givenFace);

    public:
    void load(std::string filename, unsigned int* VAO, unsigned int* VBO, unsigned int* EBO);

    std::shared_ptr<float[]> getVertsArray();
    std::shared_ptr<unsigned int[]> getIndicesArray();
    unsigned int getNumVerts();
    bool getWireframe() { return wireframe; };

    void setWireframe(bool wireframeStatus) { wireframe = wireframeStatus; };
    
    void scaleMesh(float factor);
    void rotateMesh(float theta, MeshRotation rotationType);
    void rotateMeshGpu(float theta, VAOContainer::MeshRotation rotationType);
    void drawGlMesh();
};

#endif // VAOCONTAINER_H