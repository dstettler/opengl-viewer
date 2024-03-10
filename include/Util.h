#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <vector>

#include "VAOContainer.h"

#include <glm/vec3.hpp>

std::vector<std::string> splitStringAtDelim(std::string str, char delimiter);

std::string readFileToString(std::string filename);

glm::vec3 rotateAboutZ(float theta, glm::vec3 oldVec);
glm::vec3 rotateAboutX(float theta, glm::vec3 oldVec);
glm::vec3 rotateAboutY(float theta, glm::vec3 oldVec);
std::vector<Face> triangulateFace(Face givenFace);

#endif // UTIL_H