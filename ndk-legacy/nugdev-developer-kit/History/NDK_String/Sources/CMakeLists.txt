cmake_minimum_required(VERSION 3.26)
project(nugdev.lib.NDK_String)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library( ${PROJECT_NAME} SHARED
        Charactor/Charactor.cpp
        Charactor/Charactor.hpp
        Charactor/CharactorDef.h
        StringDef.h
        Charactor/Interface/Charactor.h
        String/String.cpp
        String/String.h
        String/Interface/String.h
        Container/Array.cpp
        Container/Array.h
)