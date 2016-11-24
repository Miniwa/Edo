#ifndef EDO_BYTEBUF_HPP
#define EDO_BYTEBUF_HPP

#include <vector>

#include "edo/base/types.hpp"
#include "edo/base/strings.hpp"
#include "edo/base/endian.hpp"

namespace std
{
    class out_of_range;
}

namespace edo
{
    /// A endian aware buffer of bytes
    class Bytebuf
    {
    public:
        /// Default constructor
        Bytebuf();

        /// Endian constructor
        Bytebuf(endianness order);

        /// Returns the endianness of the buffer
        endianness get_endianness();

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
        const byte* data();

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
            const byte* data,
            const std::size_t length
        );

        /// Appends an array of bytes to the buffer and advances the buffer
        /// position by length bytes
        /// @param data Pointer to the data to append
        /// @param length The length of the data array
        void put(const byte* data, const std::size_t length);

        /// Appends a vector of bytes to the buffer
        void put(const std::size_t index, const std::vector<byte>& data);
        void put(const std::vector<byte>& data);

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
        template<typename T>
        T get(const std::size_t index)
        {
            T val = internal_get<T>(index);
            return edo::order_to_native(val, order);
        }

        /// Gets an object of type T from the buffer and advances the buffer
        /// position by sizeof(T) bytes
        /// @throws out_of_range If requested type is too large
        template<typename T>
        T get()
        {
            T val = internal_get<T>();
            return edo::order_to_native(val, order);
        }

        /// Gets a float from given index
        float get_f(const std::size_t index);

        /// Gets a float from the buffer and advances the buffer position by
        /// 4 bytes
        float get_f();

        /// Gets a double from given index
        double get_d(const std::size_t index);

        /// Gets a double from the buffer and advances the buffer position by
        /// 8 bytes
        double get_d();

    private:
        template<typename T>
        T internal_get(const std::size_t index)
        {
            if(index < 0 || index > size())
                throw std::out_of_range(INDEX_OUT_OF_RANGE);

            std::size_t type_size = sizeof(T);
            if(index + type_size > size())
                throw std::out_of_range(OPERATION_EXCEEDS_SIZE);

            return *(T*)&buffer[index];
        }

        template<typename T>
        T internal_get()
        {
            T res = internal_get<T>(get_pos());
            move(sizeof(T));

            return res;
        }

        std::vector<byte> buffer;
        edo::endianness order;
        std::size_t position;
    };
}
#endif
