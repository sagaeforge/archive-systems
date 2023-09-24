//
// Created by Jinwon Choi on 9/24/23.
//

#pragma once

#include "../Collection/Collection.h"

namespace ndk::structure
{
    template <typename T> class Iterator;
    template <typename T> class ReverseIterator;

    template <typename T>
    class IteratorFactory
    {
    protected:
        IteratorFactory() = default;
        virtual ~IteratorFactory() = default;

    public:
        virtual Iterator<T> begin() noexcept = 0;
        virtual Iterator<T> end() noexcept = 0;
        virtual ReverseIterator<T> rbegin() noexcept = 0;
        virtual ReverseIterator<T> rend() noexcept = 0;

    public:
        virtual key_t Next(key_t key) noexcept = 0;
        virtual key_t Prev(key_t key) noexcept = 0;
        virtual key_t Move(key_t key, const index_t& offset, IteratorSeek seek = IteratorSeek::Current) noexcept = 0;
        virtual bool Valid(key_t key) noexcept = 0;
        virtual bool NotValid(key_t key) noexcept = 0;

    public:
        virtual T Get(key_t key) = 0;
        virtual T& GetRef(key_t key) = 0;
        virtual T* GetPtr(key_t key) = 0;
    };


}

