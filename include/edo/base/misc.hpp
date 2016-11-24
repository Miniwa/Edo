#ifndef EDO_MISC_HPP
#define EDO_MISC_HPP

#include <vector>
#include <string>

#include "edo/base/types.hpp"

#define EDO_ADDR(value) (reinterpret_cast<edo::memaddr>(&value))

namespace edo
{
    /// Returns the result of splitting a given string at a given delim
    std::vector<std::string> split(const std::string& str, const char delim);

    /// Offsets a memory address by a given amount of offsets
    memaddr follow(
        memaddr base,
        std::vector<memoffset>::iterator begin,
        std::vector<memoffset>::iterator end
    );
}
#endif
