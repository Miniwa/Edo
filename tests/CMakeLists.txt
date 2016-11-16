cmake_minimum_required(VERSION 3.1)
project(edo-tests)

if(NOT edo_BOOST_STATIC)
    add_definitions(-DBOOST_TEST_DYN_LINK)
endif(NOT edo_BOOST_STATIC)

add_subdirectory(edo-base)
