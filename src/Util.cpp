#include "Util.h"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

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
