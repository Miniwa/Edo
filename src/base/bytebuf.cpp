#include <stdexcept>
#include <cstring>

#include "edo/base/bytebuf.hpp"

// Helper to define all put operations of basic types
#define PUT(type)\
void edo::Bytebuf::put(const std::size_t index, const type value)\
{\
    put(index, reinterpret_cast<const uint8_t*>(&value), sizeof(type));\
}\
\
void edo::Bytebuf::put(const type value)\
{\
    put(reinterpret_cast<const uint8_t*>(&value), sizeof(type));\
}\

edo::Bytebuf::Bytebuf()
{
    buffer = std::vector<uint8_t>();
    position = 0;
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

void edo::Bytebuf::resize(const std::size_t new_size)
{
	buffer.resize(new_size, 0);
}

void edo::Bytebuf::pad(const std::size_t byte_count)
{
	buffer.resize(buffer.size() + byte_count, 0);
}

void edo::Bytebuf::clear()
{
    buffer.clear();
    rewind();
}

const uint8_t* edo::Bytebuf::data()
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

void edo::Bytebuf::put(const std::size_t index, const uint8_t* data,
    const std::size_t length)
{
    if(index > size())
        throw std::out_of_range(OPERATION_EXCEEDS_SIZE);

    auto it = buffer.begin() + index;
    buffer.insert(it, data, data + length);
}

void edo::Bytebuf::put(const uint8_t* data, const std::size_t length)
{
    put(get_pos(), data, length);
    move(length);
}

void edo::Bytebuf::put(const std::size_t index, const std::vector<uint8_t>& data)
{
    put(index, data.data(), data.size());
}

void edo::Bytebuf::put(const std::vector<uint8_t>& data)
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

PUT(float)

PUT(double)
