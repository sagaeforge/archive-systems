//
// Created by Jinwon Choi on 10/6/23.
//

#pragma once

#include <unordered_map>

namespace ndk::Reflect {

    constexpr static unsigned int BUFFER_SIZE = 256;

    struct ReflectData
    {
        unsigned int m_offset;
        unsigned int m_size;
        char m_type[BUFFER_SIZE];
        char m_name[BUFFER_SIZE];
    };

    template <typename self>
    struct enable_reflect
    {
    public:
        enable_reflect() = default;

    private:
        inline static std::unordered_map<const char*, const ReflectData * const> m_reflectionMap{};

    protected:
        static void RegisterMemberValue(const char * name, const char * type, const unsigned int offset, const unsigned int size)
        {
            const auto itr = m_reflectionMap.find(name);
            if (itr != m_reflectionMap.end())
            {
                return;
            }

            auto reflectData = new ReflectData{};
            reflectData->m_offset = offset;
            reflectData->m_size = size;
            strcpy(reflectData->m_type, type);
            strcpy(reflectData->m_name, name);

            m_reflectionMap.emplace(name, reflectData);
        }
        static const ReflectData * const GetReflectionData(const char * name)
        {
            const auto itr = m_reflectionMap.find(name);
            if (itr == m_reflectionMap.end())
            {
                return nullptr;
            }

            return itr->second;
        }
    };


}