#include <stdexcept>
#include <cstring>

#include "bytebuf.hpp"

// Helpers to define all put operations of basic types
#define PUT(type)\
void edo::Bytebuf::put(const std::size_t index, const type value)\
{\
    type aware_value = edo::native_to_order(value, order);\
\
    put(index, reinterpret_cast<const byte*>(\
        &aware_value),\
        sizeof(type)\
    );\
}\
\
void edo::Bytebuf::put(const type value)\
{\
    type aware_value = edo::native_to_order(value, order);\
\
    put(reinterpret_cast<const byte*>(&aware_value), sizeof(type));\
}\

edo::Bytebuf::Bytebuf()
{
    buffer = std::vector<byte>();
    order = edo::endianness::native;
    position = 0;
}

void edo::Bytebuf::set_endianness(edo::endianness new_order)
{
    order = new_order;
}

edo::endianness edo::Bytebuf::get_endianness()
{
    return order;
}

std::size_t edo::Bytebuf::size()
{
    return buffer.size();
}

std::size_t edo::Bytebuf::capacity()
{
    return buffer.capacity();
}

void edo::Bytebuf::reserve(const std::size_t new_size)
{
    buffer.reserve(new_size);
}

void edo::Bytebuf::clear()
{
    buffer.clear();
    rewind();
}

const edo::byte* edo::Bytebuf::data()
{
    return buffer.data();
}

void edo::Bytebuf::set_pos(const std::size_t pos)
{
    if(pos > capacity())
        throw std::out_of_range(OPERATION_EXCEEDS_CAPACITY);

    position = pos;
}

std::size_t edo::Bytebuf::get_pos()
{
    return position;
}

void edo::Bytebuf::move(const std::size_t offset)
{
    set_pos(get_pos() + offset);
}

void edo::Bytebuf::rewind()
{
    set_pos(0);
}

void edo::Bytebuf::put(const std::size_t index, const byte* data,
                            const std::size_t length)
{
    if(index > size())
        throw std::out_of_range(OPERATION_EXCEEDS_SIZE);

    auto it = buffer.begin() + index;
    buffer.insert(it, data, data + length);
}

void edo::Bytebuf::put(const byte* data, const std::size_t length)
{
    put(get_pos(), data, length);
    move(length);
}

void edo::Bytebuf::put(const std::size_t index, const std::vector<byte>& data)
{
    put(index, data.data(), data.size());
}

void edo::Bytebuf::put(const std::vector<byte>& data)
{
    put(data.data(), data.size());
}

PUT(int8_t)

PUT(int16_t)

PUT(int32_t)

PUT(int64_t)

PUT(uint8_t)

PUT(uint16_t)

PUT(uint32_t)

PUT(uint64_t)

void edo::Bytebuf::put(const std::size_t index, const float value)
{
    uint32_t serialized = *reinterpret_cast<const uint32_t*>(&value);
    put(index, serialized);
}

void edo::Bytebuf::put(const float value)
{
    uint32_t serialized = *reinterpret_cast<const uint32_t*>(&value);
    put(serialized);
}

void edo::Bytebuf::put(const std::size_t index, const double value)
{
    uint64_t serialized = *reinterpret_cast<const uint64_t*>(&value);
    put(index, serialized);
}

void edo::Bytebuf::put(const double value)
{
    uint64_t serialized = *reinterpret_cast<const uint64_t*>(&value);
    put(serialized);
}

float edo::Bytebuf::get_f(const std::size_t index)
{
    uint32_t serialized = get<uint32_t>(index);
    return *reinterpret_cast<float*>(&serialized);
}

float edo::Bytebuf::get_f()
{
    uint32_t serialized = get<uint32_t>();
    return *reinterpret_cast<float*>(&serialized);
}

double edo::Bytebuf::get_d(const std::size_t index)
{
    uint64_t serialized = get<uint64_t>(index);
    return *reinterpret_cast<double*>(&serialized);
}

double edo::Bytebuf::get_d()
{
    uint64_t serialized = get<uint64_t>();
    return *reinterpret_cast<double*>(&serialized);
}
