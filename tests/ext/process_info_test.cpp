#include <boost/test/unit_test.hpp>
#include "edo/ext/process_info.hpp"

BOOST_AUTO_TEST_SUITE(process_info_tests)

BOOST_AUTO_TEST_CASE(test_constructor_defaults)
{
    edo::ProcessInfo pi = edo::ProcessInfo("test.exe", 10, 1, 5);

    BOOST_REQUIRE_EQUAL(pi.get_name(), "test.exe");
    BOOST_REQUIRE_EQUAL(pi.process_id, 10);
    BOOST_REQUIRE_EQUAL(pi.parent_process_id, 1);
    BOOST_REQUIRE_EQUAL(pi.thread_count, 5);
}


BOOST_AUTO_TEST_SUITE_END()
