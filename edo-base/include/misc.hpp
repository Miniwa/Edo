#ifndef EDO_MISC_HPP
#define EDO_MISC_HPP

#include <vector>
#include <string>

namespace edo
{
    /// Returns the result of splitting a given string at a given delim
    std::vector<std::string> split(const std::string& str, const char delim);
}
#endif
