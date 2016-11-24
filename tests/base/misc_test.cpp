#include <boost/test/unit_test.hpp>
#include "edo/base/misc.hpp"

struct FollowFixture
{
        FollowFixture()
        {
            offs = std::vector<edo::memoffset>();
        }

        std::vector<edo::memoffset> offs;
};

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

BOOST_FIXTURE_TEST_CASE(test_follow_with_no_offsets, FollowFixture)
{
    int32_t i = 10;
    edo::memaddr res = edo::follow(EDO_ADDR(i), offs.begin(), offs.end());

    BOOST_REQUIRE_EQUAL(&i, reinterpret_cast<int32_t*>(res));
}

BOOST_FIXTURE_TEST_CASE(test_follow_with_multiple_offsets, FollowFixture)
{
    int32_t i = 10;
    int32_t* p = &i;
    int32_t** pp = &p;

    offs.push_back(0);
    offs.push_back(0);

    edo::memaddr res = edo::follow(EDO_ADDR(pp), offs.begin(), offs.end());

    BOOST_REQUIRE_EQUAL(reinterpret_cast<int32_t*>(res), &i);
}

BOOST_FIXTURE_TEST_CASE(test_follow_with_non_zero_offset, FollowFixture)
{
    int32_t val = 1;
    int32_t* ptr[] = {NULL, &val};

    offs.push_back(sizeof(int32_t*));

    edo::memaddr res = edo::follow(EDO_ADDR(ptr), offs.begin(), offs.end());

    BOOST_REQUIRE_EQUAL(reinterpret_cast<int32_t*>(res), &val);
}

BOOST_AUTO_TEST_SUITE_END()
