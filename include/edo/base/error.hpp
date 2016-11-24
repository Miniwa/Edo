#ifndef EDO_ERROR_HPP
#define EDO_ERROR_HPP

#include <stdexcept>

namespace edo
{
    /// An error raised by the edo framework
    class EdoError : public std::runtime_error
    {
    public:
        EdoError(const std::string& msg);
    };

    /// A security-related error
    class SecurityError : public EdoError
    {
    public:
        SecurityError(const std::string& msg);
    };

    /// An error for when a given resource cant be found
    class NotFoundError : public EdoError
    {
    public:
        NotFoundError(const std::string& msg);
    };
}
#endif
