//
// Created by Jinwon Choi on 9/16/23.
//

#pragma once

#include <ostream>
#include "./Interface/Charactor.h"
#include "./CharactorDef.h"

namespace ndk::lib::string {

template <typename T, StringEndian Endian = StringEndian::Big>
class Charactor : public ICharactor
{
public: // const
    static constexpr T npos = -1;
    using value = T;

public: // Constructor / Destructor
    constexpr Charactor() noexcept = default;
    constexpr Charactor(const Charactor& rhs) noexcept
        : m_value{ rhs.m_value } {}
    constexpr Charactor(const value& rhs) noexcept
        : m_value{ rhs } {}
    virtual ~Charactor() noexcept = default;

public: // getter / setter
    constexpr value getValue() const noexcept
    {
        return m_value;
    }
    constexpr void setValue(const value& rhs) noexcept
    {
        m_value = rhs;
    }

public: // Static Method
    static bool Compare(const Charactor& lhs, const Charactor& rhs) noexcept
    {
        return lhs.m_value == rhs.m_value;
    }
    static bool isNone(const Charactor& rhs) noexcept
    {
        return rhs.m_value == static_cast<value>(0);
    }
    static bool isRangeOf(const Charactor& rhs, const Charactor& start, const Charactor& end) noexcept
    {
        return rhs.m_value >= start.m_value && rhs.m_value <= end.m_value;
    }
    static bool isLower(const Charactor& rhs) noexcept
    {
        return isRangeOf(rhs, {'a'}, {'z'});
    }
    static bool isUpper(const Charactor& rhs) noexcept
    {
        return isRangeOf(rhs, {'A'}, {'Z'});
    }
    static bool isAlpha(const Charactor& rhs) noexcept
    {
        return isLower(rhs) || isUpper(rhs);
    }
    static bool isNumber(const Charactor& rhs) noexcept
    {
        return isRangeOf(rhs, {'0'}, {'9'});
    }
    static bool isAlphaNumber(const Charactor& rhs) noexcept
    {
        return isAlpha(rhs) || isNumber(rhs);
    }
    static bool isBin(const Charactor& rhs) noexcept
    {
        return isRangeOf(rhs, {'0'}, {'1'});
    }
    static bool isOct(const Charactor& rhs) noexcept
    {
        return isBin(rhs) || isRangeOf(rhs, {'2'}, {'7'});
    }
    static bool isHex(const Charactor& rhs) noexcept
    {
        return isOct(rhs) || isRangeOf(rhs, {'8'}, {'9'}) ||
               isRangeOf(rhs, {'a'}, {'f'}) || isRangeOf(rhs, {'A'}, {'F'});
    }
    static bool isSpace(const Charactor& rhs) noexcept
    {
        return rhs.m_value == static_cast<value>(' ');
    }
    static bool isTab(const Charactor& rhs) noexcept
    {
        return rhs.m_value == static_cast<value>('\t');
    }
    static bool isVerticalTab(const Charactor& rhs) noexcept
    {
        return rhs.m_value == static_cast<value>('\v');
    }
    static bool isNewLine(const Charactor& rhs) noexcept
    {
        return rhs.m_value == static_cast<value>('\n');
    }
    static bool isReturn(const Charactor& rhs) noexcept
    {
        return rhs.m_value == static_cast<value>('\r');
    }
    static bool isLineFeed(const Charactor& rhs) noexcept
    {
        return rhs.m_value == static_cast<value>('\f');
    }
    static bool isBlank(const Charactor& rhs) noexcept
    {
        return isSpace(rhs) ||
               isTab(rhs) ||
               isVerticalTab(rhs) ||
               isReturn(rhs) ||
               isLineFeed(rhs) ||
               isNewLine(rhs);
    }
    static bool isPunctuation(const Charactor& rhs) noexcept
    {
        return isRangeOf(rhs, {'!'}, {'/'}) ||
               isRangeOf(rhs, {':'}, {'@'}) ||
               isRangeOf(rhs, {'['}, {'`'}) ||
               isRangeOf(rhs, {'{'}, {'~'});
    }
    static bool isControl(const Charactor& rhs) noexcept
    {
        return isRangeOf(rhs, {0x00}, {0x1F}) ||
               isRangeOf(rhs, {0x7F}, {0x9F});
    }
    static bool isPrintable(const Charactor& rhs) noexcept
    {
        return !isControl(rhs);
    }
    static Charactor toLower(const Charactor& rhs) noexcept
    {
        if (isAlphaUpper(rhs))
        {
            return { rhs.m_value + 0x20 };
        }
        return rhs;
    }
    static Charactor toUpper(const Charactor& rhs) noexcept
    {
        if (isAlphaLower(rhs))
        {
            return { rhs.m_value - 0x20 };
        }
        return rhs;
    }
    static int toInt(const Charactor& rhs) noexcept
    {
        return static_cast<int>(rhs.m_value);
    }

public: // Method
    bool Equal(const Charactor& rhs) const noexcept
    {
        return Compare(*this, rhs);
    }
    bool isNone() const noexcept
    {
        return isNone(*this);
    }
    bool isLower() const noexcept
    {
        return isLower(*this);
    }
    bool isUpper() const noexcept
    {
        return isUpper(*this);
    }
    bool isAlpha() const noexcept
    {
        return isAlpha(*this);
    }
    bool isNumber() const noexcept
    {
        return isNumber(*this);
    }
    bool isAlphaNumber() const noexcept
    {
        return isAlphaNumber(*this);
    }
    bool isBin() const noexcept
    {
        return isBin(*this);
    }
    bool isOct() const noexcept
    {
        return isOct(*this);
    }
    bool isHex() const noexcept
    {
        return isHex(*this);
    }
    bool isSpace() const noexcept
    {
        return isSpace(*this);
    }
    bool isTab() const noexcept
    {
        return isTab(*this);
    }
    bool isVerticalTab() const noexcept
    {
        return isVerticalTab(*this);
    }
    bool isNewLine() const noexcept
    {
        return isNewLine(*this);
    }
    bool isReturn() const noexcept
    {
        return isReturn(*this);
    }
    bool isLineFeed() const noexcept
    {
        return isLineFeed(*this);
    }
    bool isBlank() const noexcept
    {
        return isBlank(*this);
    }
    bool isPunctuation() const noexcept
    {
        return isPunctuation(*this);
    }
    bool isControl() const noexcept
    {
        return isControl(*this);
    }
    bool isPrintable() const noexcept
    {
        return isPrintable(*this);
    }
    Charactor toLower() const noexcept
    {
        return toLower(*this);
    }
    Charactor toUpper() const noexcept
    {
        return toUpper(*this);
    }
    int toInt() const noexcept
    {
        return toInt(*this);
    }

public: // Operator
    bool operator==(const Charactor& rhs) const noexcept
    {
        return Equal(rhs);
    }
    bool operator!=(const Charactor& rhs) const noexcept
    {
        return !Equal(rhs);
    }
    bool operator==(const value& rhs) const noexcept
    {
        return m_value == rhs;
    }
    bool operator!=(const value& rhs) const noexcept
    {
        return m_value != rhs;
    }
    bool operator<(const Charactor& rhs) const noexcept
    {
        return m_value < rhs.m_value;
    }
    bool operator<=(const Charactor& rhs) const noexcept
    {
        return m_value <= rhs.m_value;
    }
    bool operator>(const Charactor& rhs) const noexcept
    {
        return m_value > rhs.m_value;
    }
    bool operator>=(const Charactor& rhs) const noexcept
    {
        return m_value >= rhs.m_value;
    }
    Charactor& operator=(const Charactor& rhs) noexcept
    {
        m_value = rhs.m_value;
        return *this;
    }
    Charactor& operator=(const value& rhs) noexcept
    {
        m_value = rhs;
        return *this;
    }
    Charactor& operator+=(const Charactor& rhs) noexcept
    {
        m_value += rhs.m_value;
        return *this;
    }
    Charactor& operator-=(const Charactor& rhs) noexcept
    {
        m_value -= rhs.m_value;
        return *this;
    }
    Charactor& operator*=(const Charactor& rhs) noexcept
    {
        m_value *= rhs.m_value;
        return *this;
    }
    Charactor& operator/=(const Charactor& rhs) noexcept
    {
        m_value /= rhs.m_value;
        return *this;
    }
    Charactor& operator%=(const Charactor& rhs) noexcept
    {
        m_value %= rhs.m_value;
        return *this;
    }
    Charactor& operator&=(const Charactor& rhs) noexcept
    {
        m_value &= rhs.m_value;
        return *this;
    }
    Charactor& operator|=(const Charactor& rhs) noexcept
    {
        m_value |= rhs.m_value;
        return *this;
    }
    Charactor& operator^=(const Charactor& rhs) noexcept
    {
        m_value ^= rhs.m_value;
        return *this;
    }
    Charactor& operator<<=(const Charactor& rhs) noexcept
    {
        m_value <<= rhs.m_value;
        return *this;
    }
    Charactor& operator>>=(const Charactor& rhs) noexcept
    {
        m_value >>= rhs.m_value;
        return *this;
    }
    Charactor operator+(const Charactor& rhs) const noexcept
    {
        return { m_value + rhs.m_value };
    }
    Charactor operator-(const Charactor& rhs) const noexcept
    {
        return { m_value - rhs.m_value };
    }
    Charactor operator*(const Charactor& rhs) const noexcept
    {
        return { m_value * rhs.m_value };
    }
    Charactor operator/(const Charactor& rhs) const noexcept
    {
        return { m_value / rhs.m_value };
    }
    Charactor operator%(const Charactor& rhs) const noexcept
    {
        return { m_value % rhs.m_value };
    }
    Charactor operator&(const Charactor& rhs) const noexcept
    {
        return { m_value & rhs.m_value };
    }
    Charactor operator|(const Charactor& rhs) const noexcept
    {
        return { m_value | rhs.m_value };
    }
    Charactor operator^(const Charactor& rhs) const noexcept
    {
        return { m_value ^ rhs.m_value };
    }
    Charactor operator>>(const Charactor& rhs) const noexcept
    {
        return { m_value >> rhs.m_value };
    }
    Charactor operator~() const noexcept
    {
        return { ~m_value };
    }
    Charactor operator!() const noexcept
    {
        return { !m_value };
    }
    Charactor& operator++() noexcept
    {
        ++m_value;
        return *this;
    }
    Charactor operator++(int) & noexcept
    {
        Charactor tmp{ m_value };
        ++m_value;
        return tmp;
    }
    Charactor& operator--() noexcept
    {
        --m_value;
        return *this;
    }
    Charactor operator--(int) & noexcept
    {
        Charactor tmp{ m_value };
        ++m_value;
        return tmp;
    }
    Charactor operator+() const noexcept
    {
        return { +m_value };
    }
    Charactor operator-() const noexcept
    {
        return { -m_value };
    }
    Charactor operator[](const Charactor& rhs) const noexcept
    {
        return { m_value[rhs.m_value] };
    }
    Charactor operator()(const Charactor& rhs) const noexcept
    {
        return { m_value(rhs.m_value) };
    }
    Charactor operator->() const noexcept
    {
        return { m_value->m_value };
    }
    Charactor operator*() const noexcept
    {
        return { *m_value };
    }
    friend std::ostream &operator<<(std::ostream &os, const Charactor &charactor) {
        os << static_cast<value>(charactor.m_value);
        return os;
    }
    friend bool operator==(const value& lhs, const Charactor& rhs) noexcept
    {
        return lhs == rhs.m_value;
    }
    friend bool operator!=(const value& lhs, const Charactor& rhs) noexcept
    {
        return lhs != rhs.m_value;
    }
    operator value() const noexcept
    {
        return getValue();
    }

public: // Other
    static constexpr size_t wordSize = sizeof(value);
    static constexpr StringEndian endian = Endian;

private:
    value m_value;
};


using Char8_t = Charactor<unsigned char>;
using Char16_t = Charactor<unsigned short>;
using Char32_t = Charactor<unsigned int>;

using Char = Char32_t;

} // namespace ndk::lib::string
