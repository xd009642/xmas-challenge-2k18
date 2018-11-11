find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

add_subdirectory(glpp)

if(NOT TARGET OpenGL::GLU)
    message(FATAL_ERROR "You need GLU to build the xmas challenge")
endif()

if(NOT TARGET OpenGL::OpenGL)
    message(FATAL_ERROR "You need OpenGL to build the xmas challenge")
endif()

set(LOCAL_SOURCE main.cpp application.cpp)

include_directories(GLUT_INCLUDE_DIR)

add_executable(xmas ${LOCAL_SOURCE})

target_link_libraries(xmas 
    glpp
    OpenGL::OpenGL 
    OpenGL::GLU
    GLUT::GLUT)
