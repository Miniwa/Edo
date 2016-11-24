#include "edo/base/error.hpp"

edo::EdoError::EdoError(const std::string& msg) : runtime_error(msg)
{

}

edo::SecurityError::SecurityError(const std::string& msg) : EdoError(msg)
{

}

edo::NotFoundError::NotFoundError(const std::string& msg) : EdoError(msg)
{

}
