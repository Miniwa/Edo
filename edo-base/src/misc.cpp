#include <sstream>
#include "misc.hpp"

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
