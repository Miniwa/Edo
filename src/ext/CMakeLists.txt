cmake_minimum_required(VERSION 3.1)
project(edo-ext)

file(GLOB edo-ext_CPP ${PROJECT_SOURCE_DIR}/*.cpp)

include_directories(${edo_INCLUDE_DIRS})

add_library(${PROJECT_NAME} SHARED ${edo-ext_CPP})
generate_export_header(${PROJECT_NAME} EXPORT_FILE_NAME
"${OUTPUT_PATH}/edo/ext/exports.hpp")
target_link_libraries(${PROJECT_NAME} edo-base)

install(TARGETS ${PROJECT_NAME} DESTINATION "${edo_LIB_INSTALL_DIR}")
