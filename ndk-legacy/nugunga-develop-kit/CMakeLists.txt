cmake_minimum_required(VERSION 3.26)
project(Nugunga_Develop_Kit)

set(CMAKE_CXX_STANDARD 20)

add_executable(Nugunga_Develop_Kit library.cpp
        Reflect/Reflection.hpp
        Reflect/Property.hpp)
