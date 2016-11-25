#ifndef EDO_EXT_PROCESS_INFO_HPP
#define EDO_EXT_PROCESS_INFO_HPP

#include <string>

#include "edo/base/types.hpp"
#include "edo/ext/exports.hpp"

namespace edo
{
    class EDO_EXT_EXPORT ProcessInfo
    {
	public:
        /// Default constructor
		ProcessInfo();

		/// Info constructor
        ProcessInfo(
            const std::string& name,
            const pid process_id,
            const pid parent,
            const uint32_t threads
        );

        /// Returns the name of the .exe that started this process
		std::string get_name();

        /// The process identifier of the process
        pid process_id;

        /// The process identifier of the parent process
        pid parent_process_id;

        /// The number of threads in this process
        uint32_t thread_count;

	private:
		std::string exe_name;
    };
}
#endif
