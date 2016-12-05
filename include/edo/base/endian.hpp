#ifndef EDO_ENDIAN_HPP
#define EDO_ENDIAN_HPP

#include <boost/endian/conversion.hpp>

namespace edo
{
    enum class endianness
    {
        big = (int)boost::endian::order::big,
        little = (int)boost::endian::order::little,
        native = (int)boost::endian::order::native,
    };

    /// Returns the big-endian representation of a given native value
    template<typename T>
    T native_to_big(T value)
    {
        return boost::endian::native_to_big(value);
    }

    /// Returns the little-endian representation of a given native value
    template<typename T>
    T native_to_little(T value)
    {
        return boost::endian::native_to_little(value);
    }

    /// Returns the given endianness representation of a given native value
    template<typename T>
    T native_to_order(T value, endianness order)
    {
        if(order == endianness::little)
        {
            return native_to_little(value);
        }
        else
        {
            return native_to_big(value);
        }
    }

    /// Returns the native representation of a given big-endian value
    template<typename T>
    T big_to_native(T value)
    {
        return boost::endian::big_to_native(value);
    }

    /// Returns the native representation of a given little-endian value
    template<typename T>
    T little_to_native(T value)
    {
        return boost::endian::little_to_native(value);
    }

    /// Returns the native representation of a given value in given endianness
    template<typename T>
    T order_to_native(T value, endianness order)
    {
        if(order == endianness::little)
        {
            return little_to_native(value);
        }
        else
        {
            return big_to_native(value);
        }
    }
}
#endif
