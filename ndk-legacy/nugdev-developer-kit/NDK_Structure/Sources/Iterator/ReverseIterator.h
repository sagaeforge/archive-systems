//
// Created by Jinwon Choi on 9/24/23.
//

#pragma once

#include "Iterator.h"

namespace ndk::structure
{
    template <typename T>
    struct ReverseIterator : public Iterator<T>
    {
    public:
        ReverseIterator() = default;
        explicit ReverseIterator(IteratorFactory<T>* factory);
        ReverseIterator(const ReverseIterator& other);
        ReverseIterator(ReverseIterator&& other) noexcept = default;
        ~ReverseIterator() = default;

        ReverseIterator& operator++() noexcept;
        ReverseIterator& operator--() noexcept;
        ReverseIterator& Next() noexcept;
        ReverseIterator& Prev() noexcept;
    };

}