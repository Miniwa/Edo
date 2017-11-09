cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_DEBUG_POSTFIX -d)
set(OUTPUT_PATH ${CMAKE_BINARY_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${OUTPUT_PATH})
set(LIBRARY_OUTPUT_PATH ${OUTPUT_PATH})

project(edo)

include(CTest)

set(EDO_VERSION_MAJOR 0)
set(EDO_VERSION_MINOR 0)
set(EDO_VERSION_REVISION 1)

set(EDO_HEADER_DIR ${PROJECT_SOURCE_DIR}/include)
set(EDO_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)
set(EDO_HEADER_INSTALL_DIR "edo")
set(EDO_LIB_INSTALL_DIR "edo/lib")

# Setup boost
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS unit_test_framework REQUIRED)

# Setup includes and gather sources
include_directories(${EDO_HEADER_DIR} ${Boost_INCLUDE_DIRS})
file(GLOB EDO_HPP ${EDO_HEADER_DIR}/**/*.hpp)
file(GLOB EDO_CPP ${EDO_SOURCE_DIR}/**/*.cpp)

# Add the library
add_library(edo STATIC ${EDO_HPP} ${EDO_CPP})

# Install edo artifacts
install(DIRECTORY ${EDO_HEADER_DIR} DESTINATION "${EDO_HEADER_INSTALL_DIR}")
install(TARGETS edo DESTINATION "${EDO_LIB_INSTALL_DIR}")

# Also compile test module
add_subdirectory(test)
