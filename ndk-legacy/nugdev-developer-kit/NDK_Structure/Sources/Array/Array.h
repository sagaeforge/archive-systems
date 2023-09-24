//
// Created by Jinwon Choi on 9/24/23.
//

#pragma once

#include "../Collection/Collection.h"
#include "../Iterator/Iterator.h"
#include "../Iterator/ReverseIterator.h"
#include "../Iterator/IteratorFactory.h"

namespace ndk::structure
{

    template <typename T>
    class Array : private IteratorFactory<T> {
    public:
        class iterator : Iterator<T> {};
        class reverse_iterator : ReverseIterator<T> {};


    };

} // namespace ndk::structure

#include "StaticArray.h"
#include "DynamicArray.h"