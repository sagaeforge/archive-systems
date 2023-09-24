//
// Created by Jinwon Choi on 9/24/23.
//

#pragma once

#include "ReverseIterator.h"

namespace ndk::structure
{
    template <typename T>
    ReverseIterator<T>::ReverseIterator(IteratorFactory<T>* factory)
        : Iterator<T>(factory)
    { }

    template <typename T>
    ReverseIterator<T>::ReverseIterator(const ReverseIterator& other)
        : Iterator<T>(other)
    { }

    template <typename T>
    ReverseIterator<T>& ReverseIterator<T>::operator++() noexcept
    {
        return Next();
    }

    template <typename T>
    ReverseIterator<T>& ReverseIterator<T>::operator--() noexcept
    {
        return Prev();
    }

    template <typename T>
    ReverseIterator<T>& ReverseIterator<T>::Next() noexcept
    {
        return *this;
    }

    template <typename T>
    ReverseIterator<T>& ReverseIterator<T>::Prev() noexcept
    {
        return *this;
    }
}