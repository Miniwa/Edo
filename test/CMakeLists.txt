cmake_minimum_required(VERSION 3.1)
project(edo-test)

# Include and library dirs
include_directories(${EDO_HEADER_DIR} ${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

# Gather sources
file(GLOB EDO_TEST_CPP ${PROJECT_SOURCE_DIR}/**/*.cpp)

# Add test executable and link it to edo and boost test
add_executable(edo-test ${EDO_TEST_CPP})
target_link_libraries(edo-test edo ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
add_test(edo-test ${OUTPUT_PATH}/edo-test)
