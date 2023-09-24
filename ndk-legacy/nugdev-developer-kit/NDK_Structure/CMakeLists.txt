cmake_minimum_required(VERSION 3.26)
project(
        nugdev.lib.NDK_Structure
        VERSION 0.0.1
        DESCRIPTION "nugdev lib) Structure Library"
        HOMEPAGE_URL ""
        LANGUAGES CXX
)


set(CMAKE_CXX_EXTENSIONS OFF)
enable_testing()

add_subdirectory( ./Sources )
add_subdirectory( ./Tests )