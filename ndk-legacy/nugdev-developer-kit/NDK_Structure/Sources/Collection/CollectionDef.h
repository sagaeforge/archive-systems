//
// Created by Jinwon Choi on 9/24/23.
//

#pragma once

namespace ndk::structure
{
    using length_t = unsigned int;
    using index_t = int;
    using key_t = unsigned int;

    enum class IteratorSeek
    {
        Begin,
        Current,
        Random,
        End,
        Unknown
    };

}