
set(CMAKE_CXX_STANDARD 17)

message(STATUS "cmake module path:${CMAKE_ROOT}")

find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(Freetype REQUIRED)
find_package(cpptoml REQUIRED)

add_subdirectory(glpp)
add_subdirectory(symbols)

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
    commands.cpp
    test_patterns.cpp)

include_directories(
    ${GLEW_INCLUDE_DIRS}
    ${GLUT_INCLUDE_DIR} 
    ${FREETYPE_INCLUDE_DIRS})

add_executable(xmas ${LOCAL_SOURCE})

target_link_libraries(xmas 
    glpp
    symbols
    OpenGL::OpenGL 
    OpenGL::GLU
    GLUT::GLUT
    ${GLEW_LIBRARIES}
    ${FREETYPE_LIBRARIES}
    cpptoml
    # Added because gcc still has filesystem as experimental
    stdc++fs)
