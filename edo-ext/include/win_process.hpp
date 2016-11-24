#ifndef EDO_EXT_WIN_PROCESS_HPP
#define EDO_EXT_WIN_PROCESS_HPP

#include "types.hpp"
#include "ext_exports.hpp"
#include "process_info.hpp"
#include <vector>
#include <string>

namespace edo
{
    // Forward declare
    class Bytebuf;
    enum class Permission;

    /// A windows process wrapper and API
    class EDO_EXT_EXPORT WinProcess
    {
    public:
        /// Default constructor
        WinProcess();

        /// Returns a list of all accessable processes currently running
        /// @throws SecurityError If you do not have permission to scan
        /// @throws EdoError If an unexpected error occurs
        static std::vector<ProcessInfo> scan();

        /// Returns whether a process is currently open or not
        bool is_open();

        /// Opens a process with given process id and given permissions
        /// @param process_id The id of the process to open
        /// @param perm The permissions to request
        /// @throws NotFoundError If no such process could be found
        /// @throws SecurityError If you do not have permission to open process
        /// @throws EdoError If an unexpected error occurs
        void open(const pid process_id, const Permission perm);

        /// Closes the currently open process
        /// Does nothing if no process is open
        void close();

        /// Returns the baseaddress of the process
        memaddr get_baseaddress();

        /// Returns the baseaddress of a given module
        /// @throws NotFoundError If no such module exists
        /// @throws SecurityError If you are not permitted to scan for modules
        /// @throws EdoError If an unexpected error occurs
        memaddr get_module_baseaddress(const std::string& module);

        /// Reads a given amount of bytes from given address into given buffer
        /// @returns True on success, false on error
        bool memread(
            const memaddr address,
            const std::size_t byte_count,
            Bytebuf& buffer
        );

        /// Reads an object of type T and places the result in given pointer
        /// @returns True on success, false on error
        template<typename T>
        bool memread(
            const memaddr address,
            const T* result
        );

        /// Reads a given amount of bytes into given buffer safely
        /// @throws EdoError If operation fails
        void safe_memread(
            const memaddr address,
            const std::size_t byte_count,
            Bytebuf& buffer
        );

        /// Reads an object of type T safely and places the result
        /// in given pointer
        /// @throws EdoError If operation fails
        template<typename T>
        void safe_memread(
            const memaddr address,
            T* result
        );

        /// Returns a buffer of given size read from given address safely
        /// @throws EdoError If operations fails
        Bytebuf safe_memread(
            const memaddr address,
            const std::size_t byte_count
        );

        /// Writes a given amount of bytes to given address
        /// @param address The address to write to
        /// @param buffer The buffer to read from
        /// @param index The index in the buffer to read from
        /// @param byte_count The amount of bytes to write
        /// @returns True on success, false on error
        bool memwrite(
            const memaddr address,
            Bytebuf& buffer,
            const std::size_t index,
            const std::size_t byte_count
        );

        /// Writes an object of type T to given address
        /// @returns True on success, false on error
        template<typename T>
        bool memwrite(
            const memaddr address,
            const T& obj
        );

        /// Writes a given amount of bytes to given address safely
        /// @param address The address to write to
        /// @param buffer The buffer to read from
        /// @param index The index in the buffer to read from
        /// @param byte_count The amount of bytes to write
        /// @throws EdoError If operation fails
        void safe_memwrite(
            const memaddr address,
            Bytebuf& buffer,
            const std::size_t index,
            const std::size_t byte_count
        );

        /// Writes an object of type T to given address
        /// @throws EdoError If operations fails
        template<typename T>
        void safe_memwrite(
            const memaddr address,
            const T& obj
        );

        /// Follows an address in the target process
        /// @see edo::follow
        memaddr follow(
            const memaddr base,
            std::vector<memoffset>::iterator begin,
            std::vector<memoffset>::iterator end
        );

		private:
			hproc open_secure_process(pid process_id, Permission perm);
			void throw_if_closed();
			void throw_if_open();

			bool process_open;
			hproc process_handle;
			ProcessInfo process_info;
    };
}
#endif
