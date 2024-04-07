#ifndef VAOCONTAINER_H
#define VAOCONTAINER_H

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <glm/vec3.hpp>
#include <glm/ext/matrix_float4x4.hpp>

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

    enum class TriMode
    {
        SeparateTris,
        IndexedTris
    };

    private:
    std::vector<glm::vec3> verts;
    std::vector<glm::vec3> normals;
    std::vector<Face> faces;

    glm::vec3 cameraPos;

    TriMode triMode;

    // For GPU calculations
    float existingGpuScaleFactor = 1.0f;
    float existingGpuXTheta = 0.0f, existingGpuYTheta = 0.0f, existingGpuZTheta = 0.0f;

    bool wireframe = false;

    unsigned int *vao, *vertexShader, *fragmentShader, *shaderProgram;
    glm::mat4 *projection;
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
    void load(std::string filename, 
        unsigned int* VAO, 
        unsigned int* VBO, 
        unsigned int* EBO, 
        unsigned int* vertexShader, 
        unsigned int* fragmentShader,
        unsigned int* shaderProgram,
        glm::mat4* projection,
        TriMode triMode = TriMode::IndexedTris);

    std::shared_ptr<float[]> getVertsArray();
    std::shared_ptr<unsigned int[]> getIndicesArray();
    unsigned int getNumVerts();
    bool getWireframe() { return wireframe; };

    void setWireframe(bool wireframeStatus) { wireframe = wireframeStatus; };

    void regenMatrices();

    glm::vec3* getCamera() { return &cameraPos; };

    float* getExistingGpuXTheta() { return &existingGpuXTheta; };
    
    void scaleMesh(float factor);
    void scaleMeshGpu(float factor);
    void rotateMesh(float theta, MeshRotation rotationType);
    void rotateMeshGpu(float theta, MeshRotation rotationType);
    void drawGlMesh();
};

#endif // VAOCONTAINER_H