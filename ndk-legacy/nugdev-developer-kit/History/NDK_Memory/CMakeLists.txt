cmake_minimum_required( VERSION 3.25 )
project(
    nugdev.NDK_Memory
    VERSION 0.0.1
    DESCRIPTION "nugdev Develop Kit) Memory Management Library"
    HOMEPAGE_URL ""
    LANGUAGES CXX
)

set ( NDK_Memory_CXX_Version 23 )
set ( NDK_Memory_CXX_Compiler_Path "/usr/bin/clang++" )


# internal setting
set ( CMAKE_CXX_STANDARD ${NDK_Memory_CXX_Version} )

add_executable( ${PROJECT_NAME} Sources/Memory.h Sources/Memory.cpp )
set_property( TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD ${NDK_Memory_CXX_Version} )
