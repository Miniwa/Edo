#include <boost/test/unit_test.hpp>
#include "edo/base/endian.hpp"

struct EndianFixture
{
    EndianFixture()
    {
        native_i = 1;
        big_i = boost::endian::native_to_big(native_i);
        little_i = boost::endian::native_to_little(native_i);
    }

    uint32_t big_i;
    uint32_t little_i;
    uint32_t native_i;
};

BOOST_FIXTURE_TEST_SUITE(endian_tests, EndianFixture)

BOOST_AUTO_TEST_CASE(test_endianness_enum)
{
    bool little = edo::endianness::little == edo::endianness::native;
    bool big = edo::endianness::big == edo::endianness::native;
    BOOST_REQUIRE(little != big);
}

BOOST_AUTO_TEST_CASE(test_native_to_big)
{
    BOOST_REQUIRE_EQUAL(edo::native_to_big(native_i), big_i);
}

BOOST_AUTO_TEST_CASE(test_native_to_little)
{
    BOOST_REQUIRE_EQUAL(edo::native_to_little(native_i), little_i);
}

BOOST_AUTO_TEST_CASE(test_native_to_order_little)
{
    BOOST_REQUIRE_EQUAL(
        edo::native_to_order(native_i, edo::endianness::little),
        little_i
    );
}

BOOST_AUTO_TEST_CASE(test_native_to_order_big)
{
    BOOST_REQUIRE_EQUAL(
        edo::native_to_order(native_i, edo::endianness::big),
        big_i
    );
}

BOOST_AUTO_TEST_CASE(test_big_to_native)
{
    BOOST_REQUIRE_EQUAL(edo::big_to_native(big_i), native_i);
}

BOOST_AUTO_TEST_CASE(test_little_to_native)
{
    BOOST_REQUIRE_EQUAL(edo::little_to_native(little_i), native_i);
}

BOOST_AUTO_TEST_CASE(test_order_to_native_little)
{
    BOOST_REQUIRE_EQUAL(
        edo::order_to_native(little_i, edo::endianness::little),
        native_i
    );
}

BOOST_AUTO_TEST_CASE(test_order_to_native_big)
{
    BOOST_REQUIRE_EQUAL(
        edo::order_to_native(big_i, edo::endianness::big),
        native_i
    );
}

BOOST_AUTO_TEST_SUITE_END()
