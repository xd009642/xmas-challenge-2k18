find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(glm REQUIRED)
find_package(Freetype REQUIRED)

if(NOT TARGET OpenGL::GLU)
    message(FATAL_ERROR "You need GLU to build the xmas challenge")
endif()

if(NOT TARGET OpenGL::OpenGL)
    message(FATAL_ERROR "You need OpenGL to build the xmas challenge")
endif()

set(LOCAL_SOURCE 
    generic_lock.cpp 
    display_list.cpp 
    font_engine.cpp
    font_atlas.cpp
    program.cpp
    wrappers.cpp)

include_directories(
    ${GLEW_INCLUDE_DIR}
    ${GLUT_INCLUDE_DIR} 
    ${FREETYPE_INCLUDE_DIRS}
    ${glm_INCLUDE_DIRS})

add_library(glpp ${LOCAL_SOURCE})

target_link_libraries(glpp
    ${GLEW_LIBRARY}
    OpenGL::OpenGL 
    OpenGL::GLU
    GLUT::GLUT
    ${FREETYPE_LIBRARIES})
