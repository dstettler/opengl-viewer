#include "VAOContainer.h"

#include "Util.h"

#include <fstream>
#include <memory>
#include <string>
#include <vector>

#include <GL/glew.h>

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <glm/common.hpp>
#include <glm/geometric.hpp>

void VAOContainer::readFromFile(std::string filename)
{
    std::ifstream in(filename);
    
    std::string line;
    while(std::getline(in, line))
    {
        auto split = splitStringAtDelim(line, ' ');
        if (split.size() <= 0)
            continue;

        std::string lineType = split[0];
        if (lineType == "v")
        {
            verts.push_back(glm::vec3(std::stof(split[1]), std::stof(split[2]), std::stof(split[3])));
        } 
        else if (lineType == "vn")
        {
            normals.push_back(glm::vec3(std::stof(split[1]), std::stof(split[2]), std::stof(split[3])));
        }
        else if (lineType == "f")
        {
            Face f;
            for (auto iter = split.begin(); iter != split.end(); ++iter)
            {
                if (iter == split.begin())
                    continue;
                
                // Mesh stores faces with just normals
                if (iter->find("//") != iter->npos)
                {
                    std::string v1, v2;
                    size_t delimIndex = iter->find("//");
                    v1 = iter->substr(0, delimIndex);
                    v2 = iter->substr(delimIndex + 2, iter->size());
                    Face::Index index;
                    index.vertex = stoi(v1) - 1;
                    index.normal = stoi(v2) - 1;
                    f.indices.push_back(index);
                }
                // Mesh stores faces with normals and textures or just textures
                else if (iter->find("/") != iter->npos)
                {
                    // For simplicity in this assignment I will *only* read the vertices in this
                    // situation
                    // TODO: Make this behavior more complex
                    std::string v1, v2;
                    size_t delimIndex = iter->find("/");
                    v1 = iter->substr(0, delimIndex);
                    v2 = v1;
                    Face::Index index;
                    index.vertex = stoi(v1) - 1;
                    index.normal = stoi(v2) - 1;
                    f.indices.push_back(index);
                }
                else
                {
                    Face::Index index;
                    index.vertex = stoi(*iter) - 1;
                    index.normal = 0;
                    f.indices.push_back(index);
                }
            }

            //auto triangulatedFaces = triangulateFace(f);
            //for (Face triangulated : triangulatedFaces)
            //    faces.push_back(triangulated);
            faces.push_back(f);
        }
    }

    in.close();
}

void VAOContainer::deinit()
{
    lastVertsArrayGenerated.reset();
    lastIndicesArrayGenerated.reset();
    verts.clear();
    normals.clear();

    isInit = false;
}

void VAOContainer::init(bool fullDeinit)
{
    if (isInit && fullDeinit)
        deinit();
    else if (!fullDeinit)
    {
        lastVertsArrayGenerated.reset();
        lastIndicesArrayGenerated.reset();
    }

    // No need to take the return value of these, since they will save the shared ptrs to the class anyways
    getVertsArray();
    getIndicesArray();

    glGenVertexArrays(1, vao);
    glGenBuffers(1, vbo);
    glGenBuffers(1, ebo);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(*vao);

    glBindBuffer(GL_ARRAY_BUFFER, *vbo);
    glBufferData(GL_ARRAY_BUFFER, verts.size() * 3 * 2 * sizeof(float), lastVertsArrayGenerated.get(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size() * 3 * sizeof(unsigned int), lastIndicesArrayGenerated.get(), GL_STATIC_DRAW);

    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    isInit = true;
}

void VAOContainer::load(std::string filename, unsigned int* VAO, unsigned int* VBO, unsigned int* EBO)
{
    if (isInit)
        deinit();

    vao = VAO;
    vbo = VBO;
    ebo = EBO;
    readFromFile(filename);
    init();
}

void VAOContainer::drawGlMesh()
{
    if (wireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else 
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glBindVertexArray(*vao);
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(faces.size()) * 2 * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

std::shared_ptr<float[]> VAOContainer::getVertsArray()
{
    if (lastVertsArrayGenerated)
        return lastVertsArrayGenerated;

    std::shared_ptr<float[]> ptr(new float[getNumVerts() * 2]);
    int i = 0;
    float r, g, b;
    r = 1.0f;
    g = 0.0f;
    b = 0.0f;
    for (glm::vec3 vec : verts)
    {
        // Vert pos
        ptr[i++] = vec.x;
        ptr[i++] = vec.y;
        ptr[i++] = vec.z;

        // Vert color
        ptr[i++] = r;
        ptr[i++] = g;
        ptr[i++] = b;

        float tempR = r;
        r = b;
        b = g;
        g = tempR;        
    }

    lastVertsArrayGenerated = ptr;
    return ptr;
}

std::shared_ptr<unsigned int[]> VAOContainer::getIndicesArray()
{
    if (lastIndicesArrayGenerated)
        return lastIndicesArrayGenerated;

    std::shared_ptr<unsigned int[]> ptr(new unsigned int[faces.size() * 3]);
    int i = 0;
    for (Face f : faces)
    {
        // Vert index
        ptr[i++] = f.indices[0].vertex;
        ptr[i++] = f.indices[1].vertex;
        ptr[i++] = f.indices[2].vertex;
    }

    lastIndicesArrayGenerated = ptr;
    return ptr;    
}

unsigned int VAOContainer::getNumVerts()
{
    return verts.size() * 3;
}

void VAOContainer::rotateMesh(float theta, VAOContainer::MeshRotation rotationType)
{
    // Implement based on axis
    for (auto iter = verts.begin(); iter != verts.end(); ++iter)
    {
        if (rotationType == MeshRotation::XAxis)
            *iter = rotateAboutX(theta, *iter);
        else if (rotationType == MeshRotation::YAxis)
            *iter = rotateAboutY(theta, *iter);
        else if (rotationType == MeshRotation::ZAxis)
            *iter = rotateAboutZ(theta, *iter);
    }
    
    // Re-generate and assign arrays
    init(false);    
}

void VAOContainer::rotateMeshGpu(float theta, VAOContainer::MeshRotation rotationType)
{
    // Implement based on axis
    for (auto iter = verts.begin(); iter != verts.end(); ++iter)
    {
        glm::vec4 rotationMatrix;
        if (rotationType == MeshRotation::XAxis)
            *iter = rotateAboutX(theta, *iter);
        else if (rotationType == MeshRotation::YAxis)
            *iter = rotateAboutY(theta, *iter);
        else if (rotationType == MeshRotation::ZAxis)
            *iter = rotateAboutZ(theta, *iter);
    }
    
    // Re-generate and assign arrays
    init(false);    
}

void VAOContainer::scaleMesh(float factor)
{
    for (auto iter = verts.begin(); iter != verts.end(); ++iter)
        *iter *= factor;
    
    // Re-generate and assign arrays
    init(false);
}

float cross2D(glm::vec2 a, glm::vec2 b)
{
    return a.x * b.y - b.x * a.y;
}

std::vector<Face> VAOContainer::triangulateFace(Face givenFace)
{
    std::vector<Face> faces;
    if (!isAlreadyTri(&givenFace))
    {
        std::vector<Face::Index> indices;
        for (auto i : givenFace.indices)
            indices.push_back(i);

        int numTris = givenFace.indices.size() - 2;
        while (indices.size() > 3)
        {
            for (auto iter = indices.begin(); iter != indices.end(); ++iter)
            {
                int a = iter->vertex;
                auto aNorm = iter->normal;
                int b;
                int bNorm;
                if (iter == indices.begin())
                {
                    b = indices.rbegin()->vertex;
                    bNorm = indices.rbegin()->normal;
                }
                else
                {
                    --iter;
                    b = iter->vertex;
                    bNorm = iter->normal;
                    ++iter;
                }

                ++iter;
                int c = iter->vertex;
                int cNorm = iter->normal;
                --iter;

                // Make all verts 2D since we're subdividing a face
                auto vertA = glm::vec2(verts[a]);
                auto vertB = glm::vec2(verts[b]);
                auto vertC = glm::vec2(verts[c]);

                auto aToB = vertB - vertA;
                auto bToC = vertC - vertB;
                auto aToC = vertB - vertA;

                if (cross2D(aToB, bToC) < 0.0f)
                    continue;

                bool ear = true;
                for (int i = 0; i < verts.size(); i++)
                {
                    if (i == a || i == b || i == c)
                        continue;

                    auto currentVert = glm::vec2(verts[i]);

                    float c1, c2, c3;
                    c1 = cross2D(vertB - vertA, currentVert - vertA);
                    c2 = cross2D(vertC - vertB, currentVert - vertB);
                    c3 = cross2D(vertA - vertC, currentVert - vertC);

                    if (c1 > 0 || c2 > 0 || c3 > 0)
                        continue;
                    else
                    {
                        ear = false;
                        break;
                    }
                }

                if (ear)
                {
                    Face newFace;
                    Face::Index i1;
                    i1.vertex = a;
                    i1.normal = aNorm;
                    Face::Index i2;
                    i2.vertex = a;
                    i2.normal = aNorm;
                    Face::Index i3;
                    i3.vertex = a;
                    i3.normal = aNorm;

                    newFace.indices.push_back(i1);
                    newFace.indices.push_back(i2);
                    newFace.indices.push_back(i3);

                    faces.push_back(newFace);

                    indices.erase(iter);
                }
            }
        }
        
        Face newFace;

        newFace.indices.push_back(indices[0]);
        newFace.indices.push_back(indices[1]);
        newFace.indices.push_back(indices[2]);

        faces.push_back(newFace);
    }
    else
    {
        faces.push_back(givenFace);
    }

    return faces;
}

bool VAOContainer::isAlreadyTri(Face *f)
{
    return f->indices.size() == 3;
}