//
// Created by Jinwon Choi on 9/18/23.
//

#pragma once

#include <type_traits>

namespace ndk::lib::algorithm::structure
{

    template <typename T, typename ...Packs>
    struct has_type_of { };

    template <typename T, typename Pack, typename ...Packs>
    struct has_type_of<T, Pack, Packs...> {
        constexpr static bool result = std::is_same<T, Pack>::value || has_type_of<T, Packs...>::result;
    };

    template <typename T, typename Pack>
    struct has_type_of<T, Pack> {
        constexpr static bool result = std::is_same<T, Pack>::value;
    };

    template <typename T, typename ...Packs>
    concept is_type_of = has_type_of<T, Packs...>::result;
    template <typename T1, typename T2>
    concept is_same_type = is_type_of<T1, T2>;

} // namespace ndk::lib::algorithm::structure
