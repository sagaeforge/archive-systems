//
// Created by Jinwon Choi on 9/16/23.
//

#include "gtest/gtest.h"

#include "Charactor/Charactor.hpp"

using namespace std;

using namespace ndk::lib::string;

TEST(CharactorTest, Create_none)
{
    Charactor<char> charactor;
    EXPECT_EQ(charactor.endian, StringEndian::Big);
}

