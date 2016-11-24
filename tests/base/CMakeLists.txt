cmake_minimum_required(VERSION 3.1)
project(edo-base-tests)

file(GLOB edo-base-tests_CPP ${PROJECT_SOURCE_DIR}/*.cpp)

include_directories(${edo_INCLUDE_DIRS})
link_directories(${edo_LIBRARY_DIRS})

add_executable(edo-base-tests ${edo-base-tests_CPP})
target_link_libraries(edo-base-tests edo-base ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

add_test(edo-base-tests ${OUTPUT_PATH}/edo-base-tests)
