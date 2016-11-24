cmake_minimum_required(VERSION 3.1)
project(edo-src)

add_subdirectory(base)
if(WIN32)
    add_subdirectory(ext)
else(WIN32)
    message("Skipped windows specific projects..")
endif(WIN32)
