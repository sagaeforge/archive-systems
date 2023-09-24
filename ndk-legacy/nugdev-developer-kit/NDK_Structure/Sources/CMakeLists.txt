cmake_minimum_required(VERSION 3.25)
project(
        nugdev.lib.NDK_Structure
        VERSION 0.0.1
        DESCRIPTION "nugdev lib) Structure Library"
        HOMEPAGE_URL ""
        LANGUAGES CXX
)

add_library(${PROJECT_NAME} STATIC
        ./core.h
        ./core.cpp

        Collection/Collection.h
        Collection/CollectionDef.h

        Iterator/Iterator.h
        Iterator/Iterator.hpp
        Iterator/IteratorFactory.h
        Iterator/ReverseIterator.h

        Array/Array.h
        Array/StaticArray.h
        Array/DynamicArray.h
        Iterator/ReverseIterator.hpp
        List/List.h
)
target_compile_options(
        ${PROJECT_NAME}
        PRIVATE -std=c++20
)