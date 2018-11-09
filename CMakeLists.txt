cmake_minimum_required(VERSION 3.11)

project(xmas-challenge)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

if(NOT TARGET OpenGL::GLU)
    message(FATAL_ERROR "You need GLU to build the xmas challenge")
endif()

if(NOT TARGET OpenGL::OpenGL)
    message(FATAL_ERROR "You need OpenGL to build the xmas challenge")
endif()

set(LOCAL_SOURCE src/main.cpp)

include_directories(GLUT_INCLUDE_DIR)

add_executable(xmas ${LOCAL_SOURCE})

target_link_libraries(xmas 
    OpenGL::OpenGL 
    OpenGL::GLU
    GLUT::GLUT)
