#ifndef EDO_EXT_PROCESS_INFO_HPP
#define EDO_EXT_PROCESS_INFO_HPP

#include <string>

#include "types.hpp"

namespace edo
{
    struct ProcessInfo
    {
        /// Default constructor
        ProcessInfo(
            const std::string& name,
            const pid process_id,
            const pid parent,
            const uint32_t threads
        );

        /// The name of the .exe that started this process
        std::string exe_name;

        /// The process identifier of the process
        pid process_id;

        /// The process identifier of the parent process
        pid parent_process_id;

        /// The number of threads in this process
        uint32_t thread_count;
    };
}
#endif
