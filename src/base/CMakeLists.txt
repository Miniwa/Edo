cmake_minimum_required(VERSION 3.1)
project(edo-base)

file(GLOB edo-base_CPP ${PROJECT_SOURCE_DIR}/*.cpp)

include_directories(${edo_INCLUDE_DIRS})

add_library(${PROJECT_NAME} STATIC ${edo-base_CPP})

install(TARGETS ${PROJECT_NAME} DESTINATION "${edo_LIB_INSTALL_DIR}")
