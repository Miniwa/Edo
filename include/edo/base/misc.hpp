#ifndef EDO_MISC_HPP
#define EDO_MISC_HPP

#include <vector>
#include <string>

#define EDO_ADDR(value) (reinterpret_cast<uint8_t*>(&value))

namespace edo
{
    /// Returns the result of splitting a given string at a given delim
    std::vector<std::string> split(const std::string& str, const char delim);

    /// Offsets a memory address by a given amount of offsets
    uint8_t* follow(
        uint8_t* address,
        std::vector<intptr_t>::iterator begin,
        std::vector<intptr_t>::iterator end
    );
}
#endif
