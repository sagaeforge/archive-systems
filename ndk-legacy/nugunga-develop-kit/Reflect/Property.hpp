//
// Created by Jinwon Choi on 10/7/23.
//

#pragma once

#include "./Reflection.hpp"

namespace ndk::Reflect {

    template <class type>
    struct PropertyPolicyGetter
    {
    public:
        PropertyPolicyGetter(const type& ref) : m_ref(ref) {}
        PropertyPolicyGetter(const PropertyPolicyGetter& rhs) : m_ref(rhs.m_ref) {}

    public:
        explicit operator type() const { return m_ref; }

    private:
        const type& m_ref;
    };

    template <typename type>
    struct PropertyPolicySetter
    {
    public:
        PropertyPolicySetter(type& ref) : m_ref(ref) {}
        PropertyPolicySetter(const PropertyPolicySetter& rhs) : m_ref(rhs.m_ref) {}

    public:
        PropertyPolicySetter& operator=(const type& rhs) { m_ref = rhs; return *this; }

    private:
        type& m_ref;
    };

    template <typename type>
    struct PropertyPolicyAll
    {
    public:
        PropertyPolicyAll(type& ref) : m_getter{ref}, m_setter{ref} {}

    public:
        explicit operator type() const { return m_getter.operator type(); }
        PropertyPolicyAll& operator=(const type& rhs) { m_setter.operator=(rhs); return *this; }

    private:
        PropertyPolicyGetter<type> m_getter;
        PropertyPolicySetter<type> m_setter;
    };

    template <typename type, template<class> class PropertyPolicy = PropertyPolicyAll>
    class Property
    {
    public:
        Property() : m_value{}, m_policy{m_value} {}
        Property(const type& value) : m_value{value}, m_policy{m_value} {}

    public:
        // Getter
        type Get() const const { return m_policy.operator type(); }
        explicit operator type() const { return m_policy.operator type(); }
        // Setter
        Property& Set(const type& rhs) { m_policy.operator=(rhs); return *this; }
        Property& operator=(const type& rhs) { m_policy.operator=(rhs); return *this; }


    private:
        type m_value;
        PropertyPolicy<type> m_policy;
    };

}