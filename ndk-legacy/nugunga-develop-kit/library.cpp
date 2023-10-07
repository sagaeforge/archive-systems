#include "library.h"
#include "./Reflect/Property.hpp"

#include <iostream>

class Test : public ndk::Reflect::enable_reflect<Test> {
    int m_a;
    std::string m_b;
public:
    int m_c;
    ndk::Reflect::Property<int> m_d;

public:
    Test() {
        ndk::Reflect::enable_reflect<Test>::RegisterMemberValue("m_a", "int", offsetof(Test, m_a), sizeof(int));
        ndk::Reflect::enable_reflect<Test>::RegisterMemberValue("m_b", "std::string", offsetof(Test, m_b), sizeof(std::string));
        ndk::Reflect::enable_reflect<Test>::RegisterMemberValue("m_c", "int", offsetof(Test, m_c), sizeof(int));
        ndk::Reflect::enable_reflect<Test>::RegisterMemberValue("m_d", "ndk::Reflect::Property<int>", offsetof(Test, m_d), sizeof(ndk::Reflect::Property<int>));
    }

    void test() const {
        std::cout << ndk::Reflect::enable_reflect<Test>::GetReflectionData("m_a")->m_size << std::endl;
        std::cout << ndk::Reflect::enable_reflect<Test>::GetReflectionData("m_b")->m_size << std::endl;
        std::cout << ndk::Reflect::enable_reflect<Test>::GetReflectionData("m_c")->m_size << std::endl;
        std::cout << ndk::Reflect::enable_reflect<Test>::GetReflectionData("m_d")->m_size << std::endl;
    }
};


int main()
{
     auto test = Test{};
    std::cout << "Hello, World!" << std::endl;

    test.test();
    test.m_d = 32;

    return 0;
}

/*
 * Property<self, type, parent = self, getter = defaultGetter<type>, setter = defaultSetter<type>> 이런 형식임.
 * - 정책(Get/Set) 설정 가능
 * - Reflect 됨.
 *
 * PropertyVar --> 값을 소유하고, Property를 따라감.
 * PropertyRef --> 기존에 멤버 변수를 참조하고 있고, Property를 따라감.
 *
 *
 *
 *
 */