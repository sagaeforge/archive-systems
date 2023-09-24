//
// Created by Jinwon Choi on 9/24/23.
//

#pragma once

#include <type_traits>

#include "Iterator.h"

namespace ndk::structure
{
    template<typename T>
    Iterator<T>::Iterator(IteratorFactory<T>* factory) : factory{factory}, key{0}
    { }

    template<typename T>
    Iterator<T>::Iterator(const Iterator& other) : factory{other.factory}, key{other.key}
    { }

    template<typename T>
    T& Iterator<T>::operator*()
    {
        return GetRef();
    }

    template<typename T>
    T* Iterator<T>::operator->() noexcept
    {
        return GetPtr();
    }

    template<typename T>
    Iterator<T>& Iterator<T>::operator++() noexcept
    {
        return Next(this);
    }

    template<typename T>
    Iterator<T>& Iterator<T>::operator--() noexcept
    {
        return Prev(this);
    }

    template<typename T>
    Iterator<T>& Iterator<T>::Next() noexcept
    {
        this->key = factory->Next(key);
        return *this;
    }

    template<typename T>
    Iterator<T>& Iterator<T>::Prev() noexcept
    {
        this->key = factory->Prev(key);
        return *this;
    }

    template<typename T>
    bool Iterator<T>::operator==(const Iterator& other) noexcept
    {
        return Compare(*this, other);
    }

    template<typename T>
    bool Iterator<T>::operator!=(const Iterator& other) noexcept
    {
        return !Compare(*this, other);
    }

    template<typename T>
    T Iterator<T>::GetValue() noexcept
    {
        return factory->Get(key);
    }

    template<typename T>
    T& Iterator<T>::GetRef() noexcept
    {
        return factory->GetRef(key);
    }

    template<typename T>
    T* Iterator<T>::GetPtr() noexcept
    {
        return factory->GetPtr(key);
    }

    template<typename T>
    bool Iterator<T>::Compare(const Iterator<T>& lhs, const Iterator<T>& rhs) noexcept
    {
        const auto isLHSValidCheck = lhs.factory.Valid(lhs.key);
        const auto isRHSValidCheck = rhs.factory.Valid(rhs.key);
        if (isLHSValidCheck == false && isRHSValidCheck == false)
            return true;

        const auto isKeyCheck = lhs.key == rhs.key;
        if (isKeyCheck == false)
            return false;

        if constexpr (isEqual<T>)
        {
            const auto isValueCheck = lhs.GetValue() == rhs.GetValue();
            if (isValueCheck == false)
                return false;
        }
        else if constexpr (std::is_pointer<T>::value)
        {
            const auto isValueCheck = lhs.GetValue() == rhs.GetValue();
            if (isValueCheck == false)
                return false;
        }
        else
        {
            constexpr static length_t len = sizeof(T);
            const char *lhsPtr = static_cast<const char *>(lhs->GetPtr());
            const char *rhsPtr = static_cast<const char *>(rhs->GetPtr());

            for (length_t i = 0; i < len; ++i)
            {
                if (lhsPtr[i] != rhsPtr[i])
                    return false;
            }
        }
        return true;
    }

    template<typename T>
    void Iterator<T>::Next(Iterator& iterator) noexcept
    {
        iterator.Next();
    }

    template<typename T>
    void Iterator<T>::Prev(Iterator& iterator) noexcept
    {
        iterator.Prev();
    }

}