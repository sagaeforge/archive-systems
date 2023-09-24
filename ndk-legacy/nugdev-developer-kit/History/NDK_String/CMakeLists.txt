cmake_minimum_required(VERSION 3.26)
project(nugdev.lib.NDK_String)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(./Sources)
add_subdirectory(./Test)

enable_testing()

#
#add_library(${PROJECT_NAME} SHARED library.cpp)
