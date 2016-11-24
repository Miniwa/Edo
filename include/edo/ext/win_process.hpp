#ifndef EDO_EXT_WIN_PROCESS_HPP
#define EDO_EXT_WIN_PROCESS_HPP

#include "edo/base/types.hpp"
#include "edo/base/strings.hpp"
#include "edo/base/misc.hpp"
#include "edo/ext/exports.hpp"
#include "edo/ext/process_info.hpp"
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

        /// Reads a given amount of bytes from given address into given buffer at given index
		/// @throws out_of_range If index is out of range
        /// @returns True on success, false on error
        bool memread(
            const memaddr address,
			Bytebuf& buffer,
			const std::size_t index,
            const std::size_t byte_count
        );

        /// Reads an object of type T and places the result in given pointer
        /// @returns True on success, false on error
        template<typename T>
		bool memread(
			const memaddr address,
			T* out
		)
		{
			internal_buffer.clear();
			internal_buffer.reserve(sizeof(T));
			bool result = memread(address, internal_buffer, 0, sizeof(T));
			if (!result)
				return false;

			// Set the out pointer
			auto ptr = internal_buffer.data();
			*out = *(T*)(ptr);

			return true;
		}

        /// Reads a given amount of bytes into given buffer at given index safely
		/// @throws out_of_range If index is out of range
        /// @throws EdoError If operation fails
        void safe_memread(
            const memaddr address,
			Bytebuf& buffer,
			const std::size_t index,
            const std::size_t byte_count
        );

        /// Reads an object of type T safely and places the result
        /// in given pointer
        /// @throws EdoError If operation fails
        template<typename T>
		void safe_memread(
			const memaddr address,
			T* out
		)
		{
			bool result = memread<T>(address, out);
			if (!result)
				throw EdoError(MEMOP_FAILED);
		}

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
		/// @throws out_of_range If index is out of range
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
			const T* obj
		)
		{
			internal_buffer.clear();
			internal_buffer.put(0, *reinterpret_cast<byte**>(&obj), sizeof(T));

			return memwrite(address, internal_buffer, 0, sizeof(T));
		}

        /// Writes a given amount of bytes to given address safely
        /// @param address The address to write to
        /// @param buffer The buffer to read from
        /// @param index The index in the buffer to read from
        /// @param byte_count The amount of bytes to write
		/// @throws out_of_range If index is out of range
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
			const T* obj
		)
		{
			bool result = memwrite<T>(address, obj);
			if (!result)
				throw EdoError(MEMOP_FAILED);
		}

        /// Follows an address in the target process
		/// AKA offsets a pointer
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
			Bytebuf internal_buffer;
    };
}
#endif
