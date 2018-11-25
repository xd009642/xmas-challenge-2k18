cmake_minimum_required(VERSION 3.11)

project(xmas-challenge)

include(clang-format.cmake)

list(APPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)

add_subdirectory(src)
