cmake_minimum_required(VERSION 3.25)
project(
        nugdev.lib.NDK_Structure
        VERSION 0.0.1
        DESCRIPTION "nugdev lib) Structure Library"
        HOMEPAGE_URL ""
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(${PROJECT_NAME} STATIC
        ./core.h
        ./core.cpp
)