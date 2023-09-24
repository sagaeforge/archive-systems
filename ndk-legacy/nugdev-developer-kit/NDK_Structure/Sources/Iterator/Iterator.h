//
// Created by Jinwon Choi on 9/24/23.
//

#pragma once

#include "../Collection/Collection.h"

#include "IteratorFactory.h"

namespace ndk::structure
{
    template <typename T>
    class IteratorFactory;

    template <typename T>
    struct Iterator
    {
    public:
        Iterator() = default;
        explicit Iterator(IteratorFactory<T>* factory);
        Iterator(const Iterator& other);
        Iterator(Iterator&& other) noexcept = default;
        virtual ~Iterator() = default;

        virtual T& operator*();
        virtual T* operator->() noexcept;

        virtual Iterator& operator++() noexcept;
        virtual Iterator& operator--() noexcept;
        virtual Iterator& Next() noexcept;
        virtual Iterator& Prev() noexcept;

        bool operator==(const Iterator& other) noexcept;
        bool operator!=(const Iterator& other) noexcept;

    public:
        T GetValue() noexcept;
        T& GetRef() noexcept;
        T* GetPtr() noexcept;

    public:
        static bool Compare(const Iterator<T>& lhs, const Iterator<T>& rhs) noexcept;
        static void Next(Iterator& iterator) noexcept;
        static void Prev(Iterator& iterator) noexcept;

    private:
        IteratorFactory<T>* factory;
        key_t key;
    };

}

#include "Iterator.hpp"