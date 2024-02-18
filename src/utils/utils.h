#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

namespace utils
{
    void trim(std::string& str);
    std::vector<std::string> split(const std::string& str, const std::string& delimiter);
    std::string join(const std::vector<std::string>& vec, const std::string& delimiter);
} // namespace utils

#endif