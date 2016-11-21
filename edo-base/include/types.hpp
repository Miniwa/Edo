#ifndef EDO_TYPES_HPP
#define EDO_TYPES_HPP

#include <stdint.h>

namespace edo
{
    /// An 8-bit byte
    using byte = uint8_t;

    /// A memory address
    using memaddr = uintptr_t;

    /// A memory offset
    using memoffset = intptr_t;

    /// A process handle
    using hproc = void*;

    /// A process id
    using pid = uint32_t;
}
#endif
