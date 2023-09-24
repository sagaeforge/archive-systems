//
// Created by Jinwon Choi on 9/24/23.
//

#pragma once

#include "Array.h"

namespace ndk::structure
{
    template <typename T, length_t N>
    class StaticArray : public Array<T>
    {

    };

}