set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)

if(NOT TARGET OpenGL::OpenGL)
    message(FATAL_ERROR "You need OpenGL to build the xmas challenge")
endif()

set(LOCAL_SOURCE 
    heading_tape.cpp
    mesh_util.cpp)

add_library(symbols ${LOCAL_SOURCE})

target_link_libraries(symbols glpp OpenGL::OpenGL)
