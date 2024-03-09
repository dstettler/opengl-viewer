#include "Util.h"

#include <cmath>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

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

// Steal math from raytracer where I already did this
// glm::vec3 rotateAboutZ(float theta)
// {
//     // glm::vec4 multiplier(std::cos(theta), -(std::sin(theta)), 0, 0, std::sin(theta))
    
// }