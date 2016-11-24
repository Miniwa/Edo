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

edo::memaddr edo::follow(
    edo::memaddr base,
    std::vector<edo::memoffset>::iterator begin,
    std::vector<edo::memoffset>::iterator end
)
{
    memaddr result = base;

    for(auto it = begin; it != end; it++)
    {
        result = *reinterpret_cast<memaddr*>(result + *it);
    }

    return result;
}
