//
// Created by Jinwon Choi on 9/24/23.
//

#pragma once

#include <concepts>

#include "CollectionDef.h"

namespace ndk::structure
{

    template <typename T>
    concept Collection = requires (T t)
    {
        { t.GetCapacity() } -> std::convertible_to<length_t>;
        { t.GetSize() } -> std::convertible_to<length_t>;
        { t.Empty() } -> std::convertible_to<bool>;
        { t.HasValue() } -> std::convertible_to<bool>;
        { t.Clear() };
        { t.begin() } -> std::convertible_to<typename T::iterator>;
        { t.end() } -> std::convertible_to<typename T::iterator>;
    };

    template <typename T, typename U>
    concept LinearCollection = Collection<T> && requires (T t)
    {
        { t.Append() };
        { t.Prepend() };
        { t.Insert() };
        { t.Remove() };
        { t.RemoveAt() };
        { t.operator[]() } -> std::convertible_to<U &>;
        { t.at() } -> std::convertible_to<U &>;
    };

    template <typename T>
    concept AssociativeCollection = Collection<T> && requires (T t)
    {
        { t.Add() };
        { t.Remove() };
        { t.RemoveKey() };
        { t.ContainsKey() };
        { t.GetKey() };
        { t.GetValue() };
    };

    template<typename T>
    concept isEqual = requires (T t)
    {
        { t == t } -> std::convertible_to<bool>;
    };

}