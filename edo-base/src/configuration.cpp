#include <sstream>
#include <stdexcept>

#include "misc.hpp"
#include "configuration.hpp"

edo::ConfigMap::ConfigMap()
{
    kv_map = std::map<std::string, std::string>();
}

void edo::ConfigMap::parse(const std::string& config_str)
{
    std::stringstream perLine;
    std::string line;
    perLine.str(config_str);

    // Iterate each line
    while(std::getline(perLine, line))
    {
        if(line == "")
            continue;

        auto split_res = edo::split(line, '=');

        if(split_res.size() == 2 && split_res[0] != "" &&
            split_res[1] != "")
        {
            kv_map.emplace(split_res[0], split_res[1]);
        }
        else
            throw std::runtime_error(MALFORMATTED_CONFIG_STR);
    }
}

std::string edo::ConfigMap::serialize()
{
    std::stringstream out;

    for (auto it = kv_map.begin(); it != kv_map.end(); it++)
    {
        out << it->first << '=' << it->second << std::endl;
    }

    return out.str();
}

std::size_t edo::ConfigMap::size()
{
    return kv_map.size();
}

void edo::ConfigMap::clear()
{
    kv_map.clear();
}

bool edo::ConfigMap::has_key(const std::string& key)
{
    auto val = kv_map.find(key);
    return val != kv_map.end();
}

std::string edo::ConfigMap::get(const std::string& key)
{
    if(!has_key(key))
        throw std::out_of_range(NONEXISTANT_KEY);

    return kv_map[key];
}

void edo::ConfigMap::put(const std::string& key, const std::string& value)
{
    if(has_key(key))
        erase(key);

    kv_map.emplace(key, value);
}

void edo::ConfigMap::erase(const std::string& key)
{
    if(!has_key(key))
        throw std::out_of_range(NONEXISTANT_KEY);

    kv_map.erase(key);
}
