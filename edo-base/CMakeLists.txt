cmake_minimum_required(VERSION 3.1)
project(edo-base)

file(GLOB edo-base_CPP ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB edo-base_HPP ${PROJECT_SOURCE_DIR}/include/*.hpp)

set(edo-base_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/include ${edo_INCLUDE_DIRS}
CACHE INTERNAL "edo-base: Include Directories" FORCE)

include_directories(${edo-base_INCLUDE_DIRS})

add_library(${PROJECT_NAME} STATIC ${edo-base_CPP} ${edo-base_HPP})

install(TARGETS ${PROJECT_NAME} DESTINATION "${edo_LIB_INSTALL_DIR}")
install(FILES ${edo-base_HPP} DESTINATION "${edo_HEADER_INSTALL_DIR}/base")
