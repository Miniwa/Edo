cmake_minimum_required(VERSION 3.1)
project(edo)

set(${PROJECT_NAME}_VERSION_MAJOR 0)
set(${PROJECT_NAME}_VERSION_MINOR 1)

set(OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${OUTPUT_PATH})
set(LIBRARY_OUTPUT_PATH ${OUTPUT_PATH})

set(${PROJECT_NAME}_LIB_INSTALL_DIR "lib")
set(${PROJECT_NAME}_HEADER_INSTALL_DIR "include/edo")

set(${PROJECT_NAME}_BOOST_STATIC 0)
set(CMAKE_CXX_STANDARD 11)

include(CTest)

add_subdirectory(edo-base)
add_subdirectory(tests)
