cmake_minimum_required(VERSION 3.26)
project(nugdev.test.NDK_String)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable( ${PROJECT_NAME}
        Charactor/CharactorTest.cpp
)
target_link_libraries(
        ${PROJECT_NAME}
        nugdev.lib.NDK_String
        GTest::gtest_main
)
target_include_directories(
        ${PROJECT_NAME}
    PRIVATE
        ../Sources/
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})