
find_path(Leap_ROOT_DIR
          NAMES include/Leap.h
          HINTS ${EXTERNAL_LIBRARY_DIR}
          PATH_SUFFIXES LeapSDK-${Leap_FIND_VERSION}
          LeapSDK)

#we should check the version.txt file here...
set(Leap_INCLUDE_DIR "${Leap_ROOT_DIR}/include" )
# default to x86
set(_bit_suffix x86)
math(EXPR BITS "8*${CMAKE_SIZEOF_VOID_P}")
# always check if they change an option to compile as different target.
if(BITS EQUAL 64 OR Leap_64_BIT)
    option(Leap_64_BIT "" ON) # make sure the option is always displayed.
    if(Leap_64_BIT)
        set(_bit_suffix x64)
    endif()
else()
    option(Leap_64_BIT "" OFF)
endif()

if(Leap_64_BIT)
    set(Leap_LIBRARIES ${Leap_ROOT_DIR}/lib/x64/libLeap.so)
else()
    set(Leap_LIBRARIES ${Leap_ROOT_DIR}/lib/x32/libLeap.so)
endif()
