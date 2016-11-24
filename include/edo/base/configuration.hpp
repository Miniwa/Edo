#ifndef EDO_CONFIGURATION_HPP
#define EDO_CONFIGURATION_HPP

#include <map>
#include <boost/lexical_cast.hpp>

#include "edo/base/types.hpp"
#include "edo/base/strings.hpp"

namespace edo
{
    /// A simple string kv-pair based configuration map
    /// In external format, pairs are separated by newline and format
    /// is as follows:
    /// {key}={value}
    class ConfigMap
    {
    public:
        /// Default constructor
        ConfigMap();

        /// Parses a configuration string and inserts the keys
        /// @throws runtime_error If malformatted config string is given
        void parse(const std::string& config_str);

        /// Serializes the map into string format (see classwide comment)
        std::string serialize();

        /// Returns the amount of key-value pairs in the map
        std::size_t size();

        /// Clears the map
        void clear();

        /// Returns whether a given key exists in the map
        bool has_key(const std::string& key);

        /// Returns the value assosciated with given key
        /// @throws out_of_range If given key does not exist in the map
        std::string get(const std::string& key);

        /// Returns the value assosciated with given key lexical casted
        /// to type T
        /// @throws out_of_range If given key does not exist in the map
        /// @throws runtime_error If the lexical casting fails
        template<typename T>
        T get(const std::string& key)
        {
            std::string val = get(key);

            try
            {
                return boost::lexical_cast<T>(val);
            }
            catch(const boost::bad_lexical_cast& e)
            {
                //Just raise runtime error instead
                throw std::runtime_error(BAD_CAST);
            }
        }

        /// Inserts or replaces a given value with given key in the map
        // TODO: And what if key already exists?
        void put(const std::string& key, const std::string& value);

        /// Inserts a given value lexical casted to type std::string
        /// with given key into the map
        /// @throws runtime_error If the lexical casting fails
        template<typename T>
        void put(const std::string& key, const T value)
        {
            std::string val;

            try
            {
                val = boost::lexical_cast<std::string>(value);
            }
            catch(const boost::bad_lexical_cast& e)
            {
                //Raise runtime error
                throw std::runtime_error(BAD_CAST);
            }

            put(key, val);
        }

        /// Erases the value with given key
        /// @throws out_of_range If given key does not exist in the map
        void erase(const std::string& key);

    private:
        std::map<std::string, std::string> kv_map;
    };
}
#endif
