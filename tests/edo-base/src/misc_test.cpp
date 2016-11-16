#include <boost/test/unit_test.hpp>
#include "misc.hpp"

BOOST_AUTO_TEST_SUITE(misc_tests)

BOOST_AUTO_TEST_CASE(test_split_with_two_values)
{
    auto res = edo::split("key=value", '=');

    BOOST_REQUIRE_EQUAL(res.size(), 2);
    BOOST_REQUIRE_EQUAL(res[0], "key");
    BOOST_REQUIRE_EQUAL(res[1], "value");
}

BOOST_AUTO_TEST_CASE(test_split_with_one_value)
{
    auto res = edo::split("key=value", '_');

    BOOST_REQUIRE_EQUAL(res.size(), 1);
    BOOST_REQUIRE_EQUAL(res[0], "key=value");
}

BOOST_AUTO_TEST_CASE(test_split_with_multiple_empty_strings)
{
    auto res = edo::split("===", '=');

    BOOST_REQUIRE_EQUAL(res.size(), 3);
}

BOOST_AUTO_TEST_CASE(test_split_empty_string)
{
    auto res = edo::split("", '=');

    BOOST_REQUIRE_EQUAL(res.size(), 1);
}

BOOST_AUTO_TEST_SUITE_END()
