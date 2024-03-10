#include "Util.h"

#include <cmath>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <glm/vec3.hpp>

std::vector<std::string> splitStringAtDelim(std::string str, char delimiter)
{
    std::stringstream stream(str);
    std::vector<std::string> vec;
    std::string seg;
    while (std::getline(stream, seg, delimiter))
        vec.push_back(seg);

    return vec;
}

std::string readFileToString(std::string filename)
{
    std::ifstream stream(filename);
    std::string builder = std::string();
    std::string line;
    while (getline(stream, line))
    {
        builder += line + '\n';
    }

    stream.close();

    return builder;
}

glm::vec3 rotateAboutZ(float theta, glm::vec3 oldVec)
{
    glm::vec3 newVec;
    newVec.x = (oldVec.x * std::cos(theta)) + (oldVec.y * -std::sin(theta)) + (oldVec.z * 0) + (1 * 0);
    newVec.y = (oldVec.x * std::sin(theta)) + (oldVec.y * std::cos(theta)) + (oldVec.z * 0) + (1 * 0);
    newVec.z = (oldVec.x * 0) + (oldVec.y * 0) + (oldVec.z * 1) + (1 * 0);
    
    return newVec;
}

glm::vec3 rotateAboutX(float theta, glm::vec3 oldVec)
{
    glm::vec3 newVec;
    newVec.x = (oldVec.x * 1) + (oldVec.y * 0) + (oldVec.z * 0) + (1 * 0);
    newVec.y = (oldVec.x * 0) + (oldVec.y * std::cos(theta)) + (oldVec.z * -std::sin(theta)) + (1 * 0);
    newVec.z = (oldVec.x * 0) + (oldVec.y * std::sin(theta)) + (oldVec.z * std::cos(theta)) + (1 * 0);
    
    return newVec;
}

glm::vec3 rotateAboutY(float theta, glm::vec3 oldVec)
{
    glm::vec3 newVec;
    newVec.x = (oldVec.x * std::cos(theta)) + (oldVec.y * 0) + (oldVec.z * std::sin(theta)) + (1 * 0);
    newVec.y = (oldVec.x * 0) + (oldVec.y * 1) + (oldVec.z * 0) + (1 * 0);
    newVec.z = (oldVec.x * -(std::sin(theta))) + (oldVec.y * 0) + (oldVec.z * std::cos(theta)) + (1 * 0);
    
    return newVec;
}