#include <boost/test/unit_test.hpp>

#include "edo/base/bytebuf.hpp"

struct BytebufFixture
{
    BytebufFixture()
    {
        b = edo::Bytebuf();
    }

    edo::Bytebuf b;
};

BOOST_FIXTURE_TEST_SUITE(bytebuf_test, BytebufFixture)

BOOST_AUTO_TEST_CASE(test_constructor_defaults)
{
    BOOST_REQUIRE(b.get_pos() == 0);
    BOOST_REQUIRE(b.size() == 0);
}

BOOST_AUTO_TEST_CASE(test_reserve_reserves_proper_capacity)
{
    b.reserve(100);
    BOOST_REQUIRE(b.capacity() >= 100);
}

BOOST_AUTO_TEST_CASE(test_positive_resize_pads_bytes)
{
	b.resize(4);
	BOOST_REQUIRE_EQUAL(b.size(), 4);
}

BOOST_AUTO_TEST_CASE(test_negative_resize_pads_bytes)
{
	b.put(400);
	b.resize(1);
	BOOST_REQUIRE_EQUAL(b.size(), 1);
}

BOOST_AUTO_TEST_CASE(test_pad_pads_correct_amount_of_bytes)
{
	b.pad(10);
	BOOST_REQUIRE_EQUAL(b.size(), 10);
}

BOOST_AUTO_TEST_CASE(test_pad_bytes_are_equal_to_zero)
{
	b.pad(1);
	BOOST_REQUIRE_EQUAL(*b.data(), NULL);
}

BOOST_AUTO_TEST_CASE(test_pad_with_zero_does_nothing)
{
	b.pad(0);
	BOOST_REQUIRE_EQUAL(b.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_clear_resets_size_and_position)
{
    b.put(10);
    b.clear();
    BOOST_REQUIRE(b.size() == 0);
    BOOST_REQUIRE(b.get_pos() == 0);
}

BOOST_AUTO_TEST_CASE(test_data_returns_proper_pointer)
{
    uint32_t i = 10;
    b.put(i);

    const uint8_t* data_ptr = b.data();
    uint32_t* res = (uint32_t*)data_ptr;

    BOOST_REQUIRE(i == *res);
}

BOOST_AUTO_TEST_CASE(test_get_set_position)
{
    b.reserve(20);
    b.set_pos(8);
    BOOST_REQUIRE(b.get_pos() == 8);
}

BOOST_AUTO_TEST_CASE(test_set_pos_throws_out_of_range_when_out_of_range)
{
    auto cap = b.capacity() + 1;
    BOOST_REQUIRE_THROW(b.set_pos(cap);, std::out_of_range);
    BOOST_REQUIRE_THROW(b.set_pos(-1), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(test_move_sets_position)
{
    b.reserve(20);
    b.set_pos(10);
    b.move(1);
    BOOST_REQUIRE(b.get_pos() == 11);

    b.move(-2);
    BOOST_REQUIRE(b.get_pos() == 9);
}

BOOST_AUTO_TEST_CASE(test_move_throws_out_of_range_when_out_of_range)
{
    auto cap = b.capacity();
    b.set_pos(cap);
    BOOST_REQUIRE_THROW(b.move(1), std::out_of_range);

    b.set_pos(0);
    BOOST_REQUIRE_THROW(b.move(-1), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(test_rewind_sets_position_to_zero)
{
    b.reserve(20);
    b.set_pos(8);
    b.rewind();
    BOOST_REQUIRE(b.get_pos() == 0);
}

BOOST_AUTO_TEST_CASE(test_put_array_of_bytes_at_index)
{
    uint8_t val[] = {10, 20, 30, 40, 50};
    b.put(0, val, 5);
    BOOST_REQUIRE(b.size() == 5);
    BOOST_REQUIRE(b.get_pos() == 0);
    BOOST_REQUIRE(b.get<uint8_t>(0) == 10);
    BOOST_REQUIRE(b.get<uint8_t>(1) == 20);
    BOOST_REQUIRE(b.get<uint8_t>(2) == 30);
    BOOST_REQUIRE(b.get<uint8_t>(3) == 40);
    BOOST_REQUIRE(b.get<uint8_t>(4) == 50);
}

BOOST_AUTO_TEST_CASE(test_put_advances_position)
{
    uint8_t val = 10;
    b.rewind();
    b.put(&val, sizeof(val));

    BOOST_REQUIRE(b.size() == 1);
    BOOST_REQUIRE(b.get_pos() == 1);
}

BOOST_AUTO_TEST_CASE(test_put_at_throws_out_of_range_when_index_exceeds_size)
{
    uint8_t val = 10;
    BOOST_REQUIRE_THROW(b.put(b.size() + 1, &val, 1), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(test_put_throws_when_index_less_than_zero)
{
    uint8_t val = 10;
    BOOST_REQUIRE_THROW(b.put(-1, &val, sizeof(val)), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(test_put_vector_of_bytes)
{
    std::vector<uint8_t> bytes;
    bytes.push_back(10);
    bytes.push_back(20);

    b.put(0, bytes);
    BOOST_REQUIRE(b.size() == 2);
    BOOST_REQUIRE(b.get_pos() == 0);
}

BOOST_AUTO_TEST_CASE(test_put_vector_of_bytes_moves_position)
{
    std::vector<uint8_t> bytes;
    bytes.push_back(10);
    bytes.push_back(20);

    b.rewind();
    b.put(bytes);
    BOOST_REQUIRE(b.size() == 2);
    BOOST_REQUIRE(b.get_pos() == 2);
}

BOOST_AUTO_TEST_CASE(test_put_t)
{
    uint32_t i = 100;
    b.put<uint32_t>(0, &i);

    BOOST_REQUIRE_EQUAL(b.get<uint32_t>(0), i);
}

BOOST_AUTO_TEST_CASE(test_put_t_advances_position)
{
    uint32_t i = 100;
    b.put<uint32_t>(&i);

    BOOST_REQUIRE_EQUAL(b.get_pos(), sizeof(uint32_t));
    b.set_pos(0);
    BOOST_REQUIRE_EQUAL(b.get<uint32_t>(), i);
}

BOOST_AUTO_TEST_CASE(test_put_generated_type)
{
    uint64_t i = 10;
    size_t index = 0;
    b.put(index, i);

    BOOST_REQUIRE(b.size() == 8);
    BOOST_REQUIRE(b.get_pos() == 0);
    BOOST_REQUIRE(b.get<uint64_t>(0) == i);
}

BOOST_AUTO_TEST_CASE(test_put_with_generated_type_moves_position)
{
    uint64_t i = 10;
    b.put(i);

    BOOST_REQUIRE(b.size() == 8);
    BOOST_REQUIRE(b.get_pos() == 8);
    BOOST_REQUIRE(b.get<uint64_t>(0) == i);
}

BOOST_AUTO_TEST_CASE(test_put_get_with_float)
{
    float i = 10.1f;
    b.put(0, i);

    BOOST_REQUIRE_EQUAL(b.size(), 4);
    BOOST_REQUIRE_EQUAL(b.get_pos(), 0);
    BOOST_REQUIRE_EQUAL(b.get<float>(0), i);
}

BOOST_AUTO_TEST_CASE(test_put_with_float_moves_position)
{
    float i = 10.1f;
    b.put(i);

    BOOST_REQUIRE_EQUAL(b.size(), 4);
    BOOST_REQUIRE_EQUAL(b.get_pos(), 4);
    BOOST_REQUIRE_EQUAL(b.get<float>(0), i);
}

BOOST_AUTO_TEST_CASE(test_put_get_with_double)
{
    double i = 10.1;
    b.put(0, i);

    BOOST_REQUIRE_EQUAL(sizeof(double), sizeof(uint64_t));
    BOOST_REQUIRE_EQUAL(b.size(), 8);
    BOOST_REQUIRE_EQUAL(b.get_pos(), 0);
    BOOST_REQUIRE_EQUAL(b.get<double>(0), i);
}

BOOST_AUTO_TEST_CASE(test_put_with_double_advances_position)
{
    double i = 10.1;
    b.put(i);

    BOOST_REQUIRE_EQUAL(b.size(), 8);
    BOOST_REQUIRE_EQUAL(b.get_pos(), 8);
    BOOST_REQUIRE_EQUAL(b.get<double>(0), i);
}

BOOST_AUTO_TEST_CASE(test_get_t)
{
    int32_t val = 10;
    b.put(0, val);

    BOOST_REQUIRE(b.get<int32_t>(0) == val);
}

BOOST_AUTO_TEST_CASE(test_get_t_advances_position)
{
    int32_t val = 10;
    b.put(0, val);

    BOOST_REQUIRE(b.get<int32_t>() == val);
    BOOST_REQUIRE(b.get_pos() == 4);
}

BOOST_AUTO_TEST_CASE(test_get_float_advances_position)
{
    float val = 10.1f;
    b.put(0, val);

    BOOST_REQUIRE_EQUAL(b.get<float>(), val);
    BOOST_REQUIRE_EQUAL(b.get_pos(), 4);
}

BOOST_AUTO_TEST_CASE(test_get_double_advances_position)
{
    double val = 10.1f;
    b.put(0, val);

    BOOST_REQUIRE_EQUAL(b.get<double>(), val);
    BOOST_REQUIRE_EQUAL(b.get_pos(), 8);
}

BOOST_AUTO_TEST_CASE(test_get_throws_when_exceeding_size)
{
    int32_t val = 10;
    b.put(0, val);

    BOOST_REQUIRE_THROW(b.get<int32_t>(1), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(test_get_throws_when_index_less_than_zero)
{
    int32_t val = 10;
    b.put(0, val);

    BOOST_REQUIRE_THROW(b.get<int32_t>(-1), std::out_of_range);
}

BOOST_AUTO_TEST_SUITE_END()
