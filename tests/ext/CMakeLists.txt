cmake_minimum_required(VERSION 3.1)
project(edo-ext-tests)

file(GLOB edo-ext-tests_CPP ${PROJECT_SOURCE_DIR}/*.cpp)
file(GLOB edo-ext-it-tests_CPP ${PROJECT_SOURCE_DIR}/it/*.cpp)

include_directories(${edo_INCLUDE_DIRS})
link_directories(${edo_LIBRARY_DIRS})

add_executable(edo-ext-tests ${edo-ext-tests_CPP})
target_link_libraries(edo-ext-tests edo-ext ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
add_test(edo-ext-tests ${OUTPUT_PATH}/edo-ext-tests)

# Also add integration test
add_executable(edo-ext-it-tests ${edo-ext-it-tests_CPP})
target_link_libraries(edo-ext-it-tests edo-ext ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
add_test(edo-ext-it-tests ${OUTPUT_PATH}/edo-ext-it-tests)
