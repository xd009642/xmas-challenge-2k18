project(input-control)

find_package(Leap REQUIRED)

include_directories(${Leap_INCLUDE_DIR})

set(LOCAL_SRC 
    leap_interface.cpp)

add_library(input-control ${LOCAL_SRC})
target_link_libraries(input-control ${Leap_LIBRARIES})
