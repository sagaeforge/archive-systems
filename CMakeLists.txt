cmake_minimum_required(VERSION 3.25)
project(nugdev_develop_kit)

# Set C++20 as the standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# add cmake module path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# vcpkg
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/vcpkg.cmake)

add_library(${PROJECT_NAME} SHARED
    ${CMAKE_CURRENT_SOURCE_DIR}/module.cpp
)
target_link_libraries(${PROJECT_NAME}
PUBLIC
    ICU::uc
    ICU::i18n
    ICU::data
    rapidjson
    stduuid
)