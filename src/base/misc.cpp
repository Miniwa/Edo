#include <sstream>
#include "edo/base/misc.hpp"

std::vector<std::string> edo::split(const std::string& str, const char delim)
{
    std::stringstream ss;
    std::string line;
    std::vector<std::string> result;
    ss.str(str);

    if(str == "")
    {
        result.push_back("");
        return result;
    }

    while(std::getline(ss, line, delim))
    {
        result.push_back(line);
    }

    return result;
}

uint8_t* edo::follow(
    uint8_t* address,
    std::vector<intptr_t>::iterator begin,
    std::vector<intptr_t>::iterator end
)
{
    uint8_t* result = address;
    for(auto it = begin; it != end; it++)
    {
        result = *reinterpret_cast<uint8_t**>(result + *it);
    }

    return result;
}
