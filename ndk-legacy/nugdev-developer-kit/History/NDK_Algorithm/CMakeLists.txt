cmake_minimum_required(VERSION 3.26)
project(NDK_Algorithm)

set(CMAKE_CXX_STANDARD 20)

add_library(NDK_Algorithm SHARED library.cpp
        Sources/Structure/Data/Container/Container.h
        Sources/Structure/Data/Container/ContainerDef.h
        Sources/Concept/Traits/IsTypeOf.h
)
