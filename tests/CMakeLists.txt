cmake_minimum_required(VERSION 3.1)
project(edo-tests)

if(edo_BOOST_STATIC)
    set(Boost_USE_STATIC_LIBS ON)
else(edo_BOOST_STATIC)
    add_definitions(-DBOOST_TEST_DYN_LINK)
endif(edo_BOOST_STATIC)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

# Expose include and library dirs
set(edo-tests_INCLUDE_DIRS ${Boost_INCLUDE_DIRS}
CACHE INTERNAL "edo-tests: Include Directories" FORCE)

set(edo-tests_LIBRARY_DIRS ${Boost_LIBRARY_DIRS}
CACHE INTERNAL "edo-tests: Library Directories" FORCE)

add_subdirectory(edo-base)
