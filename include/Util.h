#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <vector>

std::vector<std::string> splitStringAtDelim(std::string str, char delimiter);

std::string readFileToString(std::string filename);

#endif // UTIL_H