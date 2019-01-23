
set(CMAKE_CXX_STANDARD 17)

message(STATUS "cmake module path:${CMAKE_ROOT}")

find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(glm REQUIRED)
find_package(Freetype REQUIRED)
find_package(cpptoml REQUIRED)

find_package(Boost 1.60.0 REQUIRED COMPONENTS system thread) 

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREAD ON)
set(CMAKE_CXX_FLAGS " -pthread")

add_definitions(-DBOOST_ASIO_DISABLE_BOOST_REGEX=1)
add_definitions(-DBOOST_ASIO_DISABLE_BOOST_DATE_TIME=1)


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
    test_patterns.cpp
    arm_control.cpp
    joystick_interface.cpp)

include_directories(
    ${Boost_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}
    ${GLUT_INCLUDE_DIR} 
    ${FREETYPE_INCLUDE_DIRS}
    ${glm_INCLUDE_DIRS})

add_executable(xmas ${LOCAL_SOURCE})

link_directories(${Boost_LIBRARY_DIRS})

target_link_libraries(xmas 
    glpp
    symbols
    OpenGL::OpenGL 
    OpenGL::GLU
    GLUT::GLUT
    ${GLEW_LIBRARIES}
    ${FREETYPE_LIBRARIES}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_THREAD_LIBRARY}
    cpptoml
    # Added because gcc still has filesystem as experimental
    stdc++fs)
