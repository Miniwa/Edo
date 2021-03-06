#ifndef EDO_BYTEBUF_HPP
#define EDO_BYTEBUF_HPP

#include <vector>

#include "edo/base/strings.hpp"
#include "edo/base/endian.hpp"

namespace std
{
    class out_of_range;
}

namespace edo
{
    /// A buffer of bytes
    class Bytebuf
    {
    public:
        /// Default constructor
        Bytebuf();

        /// Returns the size of the buffer
        std::size_t size();

        /// Returns the capacity of the buffer
        std::size_t capacity();

        /// Reserves enough space for given amount of elements
        /// @param new_size The amount of elements to fit
        void reserve(const std::size_t new_size);

		/// Resizes the buffer to given size
		/// Will add padding bytes to the buffer if the new size is bigger than the old
		/// Reduces the buffer to its first new_size elements if new size is less than the old
		/// @param new_size The new size to set
		void resize(const std::size_t new_size);

		/// Pads the buffer with a given amount of bytes
		/// These bytes are set to 0
		/// @param byte_count The amount of bytes to pad
		void pad(const std::size_t byte_count);

        /// Clears the buffer and rewinds the buffer position
        void clear();

        /// Returns a pointer to the internal buffer
        const uint8_t* data();

        /// Sets the position of the buffer
        /// @throws out_of_range If position is outside buffer capacity
        void set_pos(const std::size_t pos);

        /// Returns the position of the buffer
        std::size_t get_pos();

        /// Moves the position by a given offset
        /// @throws out_of_range If result position is outside buffer capacity
        void move(const std::size_t offset);

        /// Sets the position of the buffer to 0
        void rewind();

        /// Inserts an array of bytes to the buffer at a given index
        /// @param index The index of where to append
        /// @param data Pointer to the data to insert
        /// @param length The length of the data array
        /// @throws out_of_range If index exceeds buffer size
        void put(
            const std::size_t index,
            const uint8_t* data,
            const std::size_t length
        );

        /// Appends an array of bytes to the buffer and advances the buffer
        /// position by length bytes
        /// @param data Pointer to the data to append
        /// @param length The length of the data array
        void put(const uint8_t* data, const std::size_t length);

        /// Appends a vector of bytes to the buffer
        void put(const std::size_t index, const std::vector<uint8_t>& data);
        void put(const std::vector<uint8_t>& data);

        /// Appends an object of type T to buffer at given index
        template<typename T>
        void put(const std::size_t index, const T* object)
        {
                put(index, reinterpret_cast<const uint8_t*>(object), sizeof(T));
        }

        /// Appends an object of type T to buffer and advances the buffer
        /// by sizeof(T) bytes
        template<typename T>
        void put(const T* object)
        {
            put(reinterpret_cast<const uint8_t*>(object), sizeof(T));
        }

        void put(const std::size_t index, const int8_t value);
        void put(const int8_t value);

        void put(const std::size_t index, const int16_t value);
        void put(const int16_t value);

        void put(const std::size_t index, const int32_t value);
        void put(const int32_t value);

        void put(const std::size_t index, const int64_t value);
        void put(const int64_t value);

        void put(const std::size_t index, const uint8_t value);
        void put(const uint8_t value);

        void put(const std::size_t index, const uint16_t value);
        void put(const uint16_t value);

        void put(const std::size_t index, const uint32_t value);
        void put(const uint32_t value);

        void put(const std::size_t index, const uint64_t value);
        void put(const uint64_t value);

        void put(const std::size_t index, const float value);
        void put(const float value);

        void put(const std::size_t index, const double value);
        void put(const double value);

        /// Gets an object of type T from given index
        /// @param index The index of where to get from
        /// @throws out_of_range If requested type is too large
        /// @throws out_of_range If index is out of range
        template<typename T>
        T get(const std::size_t index)
        {
            if(index > size())
                throw std::out_of_range(INDEX_OUT_OF_RANGE);

            std::size_t type_size = sizeof(T);
            if(index + type_size > size())
                throw std::out_of_range(OPERATION_EXCEEDS_SIZE);

            return *(T*)&buffer[index];
        }

        /// Gets an object of type T from the buffer and advances the buffer
        /// position by sizeof(T) bytes
        /// @throws out_of_range If requested type is too large
        /// @throws out_of_range If index is out of range
        template<typename T>
        T get()
        {
            T res = get<T>(get_pos());
            move(sizeof(T));

            return res;
        }

    private:
        std::vector<uint8_t> buffer;
        std::size_t position;
    };
}
#endif
