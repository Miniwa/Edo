#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

#include "edo/base/configuration.hpp"

struct ConfigurationFixture
{
    ConfigurationFixture()
    {
        conf = edo::ConfigMap();
    }

    edo::ConfigMap conf;
};

BOOST_FIXTURE_TEST_SUITE(configuration_tests, ConfigurationFixture)

BOOST_AUTO_TEST_CASE(test_parse_adds_all_keys)
{
    std::string c = "height=200\nwidth=800\nfps=60\n";
    conf.parse(c);

    BOOST_REQUIRE_EQUAL(conf.size(), 3);
    BOOST_REQUIRE_EQUAL(conf.get("height"), "200");
    BOOST_REQUIRE_EQUAL(conf.get("width"), "800");
    BOOST_REQUIRE_EQUAL(conf.get("fps"), "60");
}

BOOST_AUTO_TEST_CASE(test_parse_throws_runtime_error_when_string_malformed)
{
    BOOST_REQUIRE_THROW(conf.parse("height="), std::runtime_error);
    BOOST_REQUIRE_THROW(conf.parse("height==200"), std::runtime_error);
    BOOST_REQUIRE_THROW(conf.parse("=height"), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(test_parse_skips_empty_lines)
{
    conf.parse("fps=60\n\ncc=3\n\n");

    BOOST_REQUIRE_EQUAL(conf.size(), 2);
    BOOST_REQUIRE_EQUAL(conf.get("cc"), "3");
    BOOST_REQUIRE_EQUAL(conf.get("fps"), "60");
}

BOOST_AUTO_TEST_CASE(test_serialize_uses_proper_format)
{
    conf.put("fps", "60");
    conf.put("cc", "33");

    BOOST_REQUIRE_EQUAL(conf.serialize(), "cc=33\nfps=60\n");
}

BOOST_AUTO_TEST_CASE(test_serialize_order)
{
    conf.put("fps", "60");
    conf.put("ccc", "33");
    conf.put("cc", "33");

    BOOST_REQUIRE_EQUAL(conf.serialize(), "cc=33\nccc=33\nfps=60\n");
}

BOOST_AUTO_TEST_CASE(test_clear_resets_size)
{
    conf.put("fps", "1231");
    conf.clear();

    BOOST_REQUIRE_EQUAL(conf.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_has_key_returns_true_when_key_exists)
{
    conf.put("fps", "60");
    BOOST_REQUIRE_EQUAL(conf.has_key("fps"), true);
}

BOOST_AUTO_TEST_CASE(test_has_key_returns_false_when_key_does_not_exist)
{
    conf.put("fps", "60");
    BOOST_REQUIRE_EQUAL(conf.has_key("fpps"), false);
}

BOOST_AUTO_TEST_CASE(test_get)
{
    conf.put("fps", "60");
    BOOST_REQUIRE_EQUAL(conf.get("fps"), "60");
}

BOOST_AUTO_TEST_CASE(test_get_lex_with_int)
{
    conf.put("fps", "60");
    BOOST_REQUIRE_EQUAL(conf.get<int32_t>("fps"), 60);
}

BOOST_AUTO_TEST_CASE(test_get_lex_throws_runtime_error_when_bad_cast)
{
    conf.put("fps", "60.00012");
    BOOST_REQUIRE_THROW(conf.get<int32_t>("fps"), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(test_get_lex_with_float)
{
    conf.put("fps", "40.00123");
    BOOST_REQUIRE_CLOSE(conf.get<float>("fps"), 40.00123, 0.0001);
}

BOOST_AUTO_TEST_CASE(test_get_lex_with_bool)
{
    conf.put("enabled", "1");
    BOOST_REQUIRE_EQUAL(conf.get<bool>("enabled"), true);
}

BOOST_AUTO_TEST_CASE(test_put_replaces_old)
{
    conf.put("fps", "40");
    conf.put("fps", "60");
    BOOST_REQUIRE_EQUAL(conf.get("fps"), "60");
}

BOOST_AUTO_TEST_CASE(test_put_lex_with_int)
{
    conf.put<int32_t>("fps", 60);
    BOOST_REQUIRE_EQUAL(conf.get("fps"), "60");
}

BOOST_AUTO_TEST_CASE(test_put_lex_with_float)
{
    conf.put<float>("fps", 60.0023);
    BOOST_REQUIRE_EQUAL(conf.get("fps"), "60.0023003");
}

BOOST_AUTO_TEST_CASE(test_put_lex_with_bool)
{
    conf.put<bool>("enabled", true);
    BOOST_REQUIRE_EQUAL(conf.get("enabled"), "1");
}

BOOST_AUTO_TEST_CASE(test_erase_deletes_existing_element)
{
    conf.put("fps", "60");
    conf.erase("fps");

    BOOST_REQUIRE_EQUAL(conf.has_key("fps"), false);
}

BOOST_AUTO_TEST_CASE(test_erase_throws_out_of_range_on_nonexisting_element)
{
    BOOST_REQUIRE_THROW(conf.erase("fps"), std::out_of_range);
}

BOOST_AUTO_TEST_SUITE_END()
