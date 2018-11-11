
set(CMAKE_CXX_STANDARD 17)

message(STATUS "cmake module path:${CMAKE_ROOT}")

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(Freetype REQUIRED)
find_package(cpptoml REQUIRED)

add_subdirectory(glpp)

if(NOT TARGET OpenGL::GLU)
    message(FATAL_ERROR "You need GLU to build the xmas challenge")
endif()

if(NOT TARGET OpenGL::OpenGL)
    message(FATAL_ERROR "You need OpenGL to build the xmas challenge")
endif()

set(LOCAL_SOURCE 
    main.cpp 
    application.cpp 
    config.cpp
    commands.cpp)

include_directories(${GLUT_INCLUDE_DIR} ${FREETYPE_INCLUDE_DIRS})

add_executable(xmas ${LOCAL_SOURCE})

target_link_libraries(xmas 
    glpp
    OpenGL::OpenGL 
    OpenGL::GLU
    GLUT::GLUT
    ${FREETYPE_LIBRARIES}
    cpptoml)
