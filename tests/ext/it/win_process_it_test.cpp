#include <boost/test/unit_test.hpp>
#include <Windows.h>

#include <edo/base/error.hpp>
#include <edo/ext/win_process.hpp>
#include <edo/ext/permission.hpp>

struct WinProcessClosedFixture
{
	WinProcessClosedFixture()
	{
		proc = edo::WinProcess();
		invalid_pid = -1;
		current_pid = GetCurrentProcessId();
	}

	edo::WinProcess proc;
	edo::pid invalid_pid;
	edo::pid current_pid;
};

struct MemFixture
{
	MemFixture()
	{
		proc = edo::WinProcess();
		proc.open(GetCurrentProcessId(), edo::Permission::all);

		b = edo::Bytebuf();
		invalid_address = NULL + 1;
		i = 1337;
		offsets = std::vector<edo::memoffset>();
	}
	
	edo::WinProcess proc;
	edo::Bytebuf b;
	edo::memaddr invalid_address;
	uint32_t i;
	std::vector<edo::memoffset> offsets;
};

BOOST_AUTO_TEST_SUITE(win_process_integration_tests)

BOOST_FIXTURE_TEST_CASE(test_constructor_defaults, WinProcessClosedFixture)
{
	BOOST_REQUIRE_EQUAL(proc.is_open(), false);
}

BOOST_AUTO_TEST_CASE(test_scan_finds_current_process)
{
	auto result = edo::WinProcess::scan();
	for (auto it = result.begin(); it != result.end(); it++)
	{
		if (it->get_name() == "edo-ext-it-tests.exe")
		{
			BOOST_REQUIRE_EQUAL(it->process_id, GetCurrentProcessId());
			return;
		}
	}

	BOOST_FAIL("Scan did not find current process");
}

BOOST_FIXTURE_TEST_CASE(test_open_process, WinProcessClosedFixture)
{
	proc.open(current_pid, edo::Permission::all);
	
	BOOST_REQUIRE_EQUAL(proc.is_open(), true);
	proc.close();
	BOOST_REQUIRE_EQUAL(proc.is_open(), false);
}

BOOST_FIXTURE_TEST_CASE(test_open_process_with_invalid_pid, WinProcessClosedFixture)
{
	BOOST_REQUIRE_THROW(proc.open(invalid_pid, edo::Permission::all), edo::EdoError);
}

BOOST_FIXTURE_TEST_CASE(test_get_process_module_base_address_closed, WinProcessClosedFixture)
{
	BOOST_REQUIRE_THROW(proc.get_baseaddress(), edo::EdoError);
	BOOST_REQUIRE_THROW(proc.get_module_baseaddress("edo-ext.dll"), edo::EdoError);
}

BOOST_FIXTURE_TEST_CASE(test_get_process_module_address, WinProcessClosedFixture)
{
	proc.open(current_pid, edo::Permission::all);

	// Fetch from system under test
	edo::memaddr base = proc.get_baseaddress();
	edo::memaddr ext_dll = proc.get_module_baseaddress("edo-ext.dll");

	// Get the baseaddress from the windows api
	HMODULE base_winapi = GetModuleHandle(NULL);
	HMODULE ext_dll_winapi = GetModuleHandle("edo-ext.dll");
	edo::memaddr base_casted = *reinterpret_cast<edo::memaddr*>(&base_winapi);
	edo::memaddr ext_dll_casted = *reinterpret_cast<edo::memaddr*>(&ext_dll_winapi);

	BOOST_REQUIRE_EQUAL(base, base_casted);
	BOOST_REQUIRE_EQUAL(ext_dll, ext_dll_casted);
}

BOOST_FIXTURE_TEST_CASE(test_memread_closed, WinProcessClosedFixture)
{
	uint32_t i = 0;
	edo::Bytebuf b;
	BOOST_REQUIRE_THROW(proc.memread(EDO_ADDR(i), b, 0, sizeof(uint32_t)), edo::EdoError);
}

BOOST_FIXTURE_TEST_CASE(test_memread, MemFixture)
{
	bool result = proc.memread(EDO_ADDR(i), b, 0, sizeof(i));

	BOOST_REQUIRE(result);
	BOOST_REQUIRE_EQUAL(b.size(), sizeof(i));
	BOOST_REQUIRE_EQUAL(b.get<uint32_t>(0), i);
}

BOOST_FIXTURE_TEST_CASE(test_memread_does_not_resize_when_data_fits, MemFixture)
{
	b.pad(10);
	bool result = proc.memread(EDO_ADDR(i), b, 0, sizeof(i));

	BOOST_REQUIRE(result);
	BOOST_REQUIRE_EQUAL(b.size(), 10);
}

BOOST_FIXTURE_TEST_CASE(test_memread_invalid_address, MemFixture)
{
	bool result = proc.memread(invalid_address, b, 0, 4);
	BOOST_REQUIRE(!result);
}

BOOST_FIXTURE_TEST_CASE(test_memread_t, MemFixture)
{
	uint32_t res = 100;
	bool result = proc.memread<uint32_t>(EDO_ADDR(i), &res);

	BOOST_REQUIRE(result);
	BOOST_REQUIRE_EQUAL(i, res);
}

BOOST_FIXTURE_TEST_CASE(test_memread_t_invalid_address, MemFixture)
{
	uint32_t res = 0;
	bool result = proc.memread(invalid_address, &res);

	BOOST_REQUIRE(!result);
}

BOOST_FIXTURE_TEST_CASE(test_safe_memread, MemFixture)
{
	proc.safe_memread(EDO_ADDR(i), b, 0, 4);
	BOOST_REQUIRE_EQUAL(b.get<uint32_t>(0), i);
}

BOOST_FIXTURE_TEST_CASE(test_safe_memread_invalid_address, MemFixture)
{
	BOOST_REQUIRE_THROW(proc.safe_memread(invalid_address, b, 0, 4), edo::EdoError);
}

BOOST_FIXTURE_TEST_CASE(test_safe_memread_t, MemFixture)
{
	uint32_t res = 0;
	proc.safe_memread<uint32_t>(EDO_ADDR(i), &res);
	BOOST_REQUIRE_EQUAL(i, res);
}

BOOST_FIXTURE_TEST_CASE(test_safe_memread_t_invalid_address, MemFixture)
{
	uint32_t res = 0;
	BOOST_REQUIRE_THROW(proc.safe_memread<uint32_t>(invalid_address, &res), edo::EdoError);
}

BOOST_FIXTURE_TEST_CASE(test_safe_memread_new_buffer, MemFixture)
{
	auto res = proc.safe_memread(EDO_ADDR(i), sizeof(uint32_t));
	BOOST_REQUIRE_EQUAL(res.get<uint32_t>(0), i);
}

BOOST_FIXTURE_TEST_CASE(test_safe_memread_new_buffer_invalid_address, MemFixture)
{
	BOOST_REQUIRE_THROW(proc.safe_memread(invalid_address, sizeof(uint32_t)), edo::EdoError);
}

BOOST_FIXTURE_TEST_CASE(test_memwrite_closed, WinProcessClosedFixture)
{
	uint32_t i = 0;
	edo::Bytebuf b;
	b.put(i);
	BOOST_REQUIRE_THROW(proc.memwrite(EDO_ADDR(i), b, 0, sizeof(uint32_t)), edo::EdoError);
}

BOOST_FIXTURE_TEST_CASE(test_memwrite, MemFixture)
{
	uint32_t res = 0;
	b.put(i);
	bool result = proc.memwrite(EDO_ADDR(res), b, 0, sizeof(uint32_t));
	
	BOOST_REQUIRE(result);
	BOOST_REQUIRE_EQUAL(res, i);
}

BOOST_FIXTURE_TEST_CASE(test_memwrite_invalid_address, MemFixture)
{
	b.put(i);
	bool result = proc.memwrite(invalid_address, b, 0, sizeof(uint32_t));

	BOOST_REQUIRE(!result);
}

BOOST_FIXTURE_TEST_CASE(test_memwrite_t, MemFixture)
{
	uint32_t res = 0;
	bool result = proc.memwrite<uint32_t>(EDO_ADDR(res), &i);

	BOOST_REQUIRE(result);
	BOOST_REQUIRE_EQUAL(res, i);
}

BOOST_FIXTURE_TEST_CASE(test_memwrite_t_invalid_address, MemFixture)
{
	uint32_t res = 0;
	bool result = proc.memwrite<uint32_t>(invalid_address, &i);

	BOOST_REQUIRE(!result);
}

BOOST_FIXTURE_TEST_CASE(test_safe_memwrite, MemFixture)
{
	uint32_t res = 0;
	b.put(i);
	proc.safe_memwrite(EDO_ADDR(res), b, 0, sizeof(uint32_t));

	BOOST_REQUIRE_EQUAL(i, res);
}

BOOST_FIXTURE_TEST_CASE(test_safe_memwrite_invalid_addr, MemFixture)
{
	uint32_t res = 0;
	b.put(i);
	BOOST_REQUIRE_THROW(proc.safe_memwrite(invalid_address, b, 0, sizeof(uint32_t)), edo::EdoError);
}

BOOST_FIXTURE_TEST_CASE(test_safe_memwrite_t, MemFixture)
{
	uint32_t res = 0;
	proc.safe_memwrite<uint32_t>(EDO_ADDR(res), &i);

	BOOST_REQUIRE_EQUAL(i, res);
}

BOOST_FIXTURE_TEST_CASE(test_safe_memwrite_t_invalid_addr, MemFixture)
{
	uint32_t res = 0;
	BOOST_REQUIRE_THROW(proc.safe_memwrite<uint32_t>(invalid_address, &i), edo::EdoError);
}

BOOST_FIXTURE_TEST_CASE(test_follow_closed, WinProcessClosedFixture)
{
	uint32_t i;
	std::vector<edo::memoffset> offsets;
	BOOST_REQUIRE_THROW(proc.follow(EDO_ADDR(i), offsets.begin(), offsets.end()), edo::EdoError);
}

BOOST_FIXTURE_TEST_CASE(test_follow_no_offsets, MemFixture)
{
	edo::memaddr res = proc.follow(EDO_ADDR(i), offsets.begin(), offsets.end());
	BOOST_REQUIRE_EQUAL(EDO_ADDR(i), res);
}

BOOST_FIXTURE_TEST_CASE(test_follow_zero_offset, MemFixture)
{
	uint32_t* p = &i;
	offsets.push_back(0);

	edo::memaddr res = proc.follow(EDO_ADDR(p), offsets.begin(), offsets.end());

	BOOST_REQUIRE_EQUAL(EDO_ADDR(i), res);
}

BOOST_FIXTURE_TEST_CASE(test_follow_non_zero_offset, MemFixture)
{
	uint32_t* ptr[] = { NULL, &i };
	offsets.push_back(sizeof(uint32_t*));

	edo::memaddr res = proc.follow(EDO_ADDR(ptr), offsets.begin(), offsets.end());

	BOOST_REQUIRE_EQUAL(EDO_ADDR(i), res);
}

BOOST_AUTO_TEST_SUITE_END()