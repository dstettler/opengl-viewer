#include "VAOContainer.h"

#include "Util.h"

#include <fstream>
#include <memory>
#include <string>
#include <vector>

#include <GL/glew.h>

#include <glm/vec3.hpp>

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
    }

    in.close();
}

void VAOContainer::deinit()
{
    lastVertsArrayGenerated.reset();
    verts.clear();
    normals.clear();

    isInit = false;
}

void VAOContainer::init(bool fullDeinit)
{
    if (isInit && fullDeinit)
        deinit();
    else if (!fullDeinit)
        lastVertsArrayGenerated.reset();

    // No need to take the return value of this, since it will save the shared ptr to the class anyways
    getVertsArray();

    glGenVertexArrays(1, vao);
    glGenBuffers(1, vbo);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(*vao);

    glBindBuffer(GL_ARRAY_BUFFER, *vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lastVertsArrayGenerated.get()), lastVertsArrayGenerated.get(), GL_STATIC_DRAW);

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

    if (wireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    isInit = true;
}

void VAOContainer::load(std::string filename, unsigned int* VAO, unsigned int* VBO)
{
    if (isInit)
        deinit();

    vao = VAO;
    vbo = vbo;
    readFromFile(filename);
    init();
}

void VAOContainer::drawGlMesh()
{
    glBindVertexArray(*vao);
    glDrawElements(GL_TRIANGLES, faces.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

std::shared_ptr<float[]> VAOContainer::getVertsArray()
{
    if (lastVertsArrayGenerated)
        return lastVertsArrayGenerated;

    std::shared_ptr<float[]> ptr(new float[getNumVerts() * 2]);
    int i = 0;
    for (glm::vec3 vec : verts)
    {
        // Vert pos
        ptr[i++] = vec.x;
        ptr[i++] = vec.y;
        ptr[i++] = vec.z;

        // Vert color
        ptr[i++] = 1.0f;
        ptr[i++] = 0.0f;
        ptr[i++] = 0.0f;
    }

    lastVertsArrayGenerated = ptr;
    return ptr;
}

unsigned int VAOContainer::getNumVerts()
{
    return verts.size() * 3;
}

void VAOContainer::rotateMesh(float theta)
{

}

void VAOContainer::scaleMesh(float factor)
{
    for (auto iter = verts.begin(); iter != verts.end(); ++iter)
        *iter *= factor;
    
    // Re-generate and assign arrays
    init(false);
}