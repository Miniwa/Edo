cmake_minimum_required(VERSION 3.1)
project(edo)

set(${PROJECT_NAME}_VERSION_MAJOR 0)
set(${PROJECT_NAME}_VERSION_MINOR 1)

set(OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${OUTPUT_PATH})
set(LIBRARY_OUTPUT_PATH ${OUTPUT_PATH})
set(CMAKE_CXX_STANDARD 11)

include(CTest)
include(GenerateExportHeader)

# Platform specific stuff
if(WIN32)
    set(VER_APPENDIX "-${edo_VERSION_MAJOR}.${edo_VERSION_MINOR}")
    set(${PROJECT_NAME}_LIB_INSTALL_DIR "lib")
    set(${PROJECT_NAME}_HEADER_INSTALL_DIR "include/edo")
    set(${PROJECT_NAME}_BOOST_STATIC ON)
endif(WIN32)
if(UNIX)
    set(${PROJECT_NAME}_LIB_INSTALL_DIR "lib")
    set(${PROJECT_NAME}_HEADER_INSTALL_DIR "include/edo")
    set(${PROJECT_NAME}_BOOST_STATIC 0)
endif(UNIX)

# Handle static boost flag
if(${PROJECT_NAME}_BOOST_STATIC)
    set(Boost_USE_STATIC_LIBS ON)
endif(${PROJECT_NAME}_BOOST_STATIC)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

# Expose include and library dirs
set(${PROJECT_NAME}_INCLUDE_DIRS ${Boost_INCLUDE_DIRS} ${OUTPUT_PATH}
CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)

set(${PROJECT_NAME}_LIBRARY_DIRS ${Boost_LIBRARY_DIRS}
CACHE INTERNAL "${PROJECT_NAME}: Library Directories" FORCE)

add_subdirectory(edo-base)
if(WIN32)
    add_subdirectory(edo-ext)
else(WIN32)
    message("Skipped windows specific projects..")
endif(WIN32)

add_subdirectory(tests)
