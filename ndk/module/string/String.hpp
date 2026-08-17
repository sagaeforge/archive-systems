#pragma once

#include <cstdint>
#include <type_traits>
#include <vector>

#include <boost/uuid.hpp>
#include <magic_enum/magic_enum.hpp>
#include <unicode/unistr.h>

namespace nugdev::ndk {

class String : public icu::UnicodeString {
  public:
    using icu::UnicodeString::UnicodeString;
    using self_t = String;
    using value_type = UChar;
    using index_type = int32_t;
    using size_type = int32_t;

  public: // constructors
    String(const std::string &str) noexcept;
    String(const icu::UnicodeString &other) noexcept;

  public: // methods
    String slice(const index_type start, const index_type end) const;
    std::vector<String> split(const String &delimiter, const size_type limit = -1) const;
    template <typename T> T to() const { return value_of<T>(*this); }

  public: // static methods
    template <typename T> static String from(const T &value) { return to_string(value); }
    template <typename T> static T to(const String &str) { return value_of<T>(str); }
};

// String 타입으로 변환하는 to_string 템플릿 함수
template <typename T> String to_string(T value);
template <> inline String to_string<int>(int value) { return String(std::to_string(value)); }
template <> inline String to_string<unsigned>(unsigned value) { return String(std::to_string(value)); }
template <> inline String to_string<long>(long value) { return String(std::to_string(value)); }
template <> inline String to_string<unsigned long>(unsigned long value) { return String(std::to_string(value)); }
template <> inline String to_string<long long>(long long value) { return String(std::to_string(value)); }
template <> inline String to_string<unsigned long long>(unsigned long long value) { return String(std::to_string(value)); }
template <> inline String to_string<float>(float value) { return String(std::to_string(value)); }
template <> inline String to_string<double>(double value) { return String(std::to_string(value)); }
template <> inline String to_string<long double>(long double value) { return String(std::to_string(value)); }
template <> inline String to_string<std::string>(std::string value) { return String(value); }
template <> inline String to_string<std::wstring>(std::wstring value) {
    std::vector<UChar32> utf32;
    utf32.reserve(value.length());

    for (const wchar_t &wc : value) {
        utf32.push_back(static_cast<UChar32>(wc));
    }

    return icu::UnicodeString::fromUTF32(utf32.data(), utf32.size());
}
template <> inline String to_string<bool>(bool value) { return String(value ? "true" : "false"); }
template <typename T>
    requires std::is_enum_v<T>
inline String to_string(T value) {
    return String(magic_enum::enum_name(value));
}
template <> inline String to_string<boost::uuids::uuid>(boost::uuids::uuid value) { return String(boost::uuids::to_string(value)); }

// String에서 다른 타입으로 변환하는 value_of 템플릿 함수
template <typename T> T value_of(const String &str);
template <> inline int value_of<int>(const String &str) { return std::stoi(str.to<std::string>()); }
template <> inline unsigned value_of<unsigned>(const String &str) { return std::stoul(str.to<std::string>()); }
template <> inline long value_of<long>(const String &str) { return std::stol(str.to<std::string>()); }
template <> inline unsigned long value_of<unsigned long>(const String &str) { return std::stoul(str.to<std::string>()); }
template <> inline long long value_of<long long>(const String &str) { return std::stoll(str.to<std::string>()); }
template <> inline unsigned long long value_of<unsigned long long>(const String &str) { return std::stoull(str.to<std::string>()); }
template <> inline float value_of<float>(const String &str) { return std::stof(str.to<std::string>()); }
template <> inline double value_of<double>(const String &str) { return std::stod(str.to<std::string>()); }
template <> inline long double value_of<long double>(const String &str) { return std::stold(str.to<std::string>()); }
template <> inline std::string value_of<std::string>(const String &str) {
    std::string result;
    str.toUTF8String(result);
    return result;
}
template <> inline std::wstring value_of<std::wstring>(const String &str) {
    if (sizeof(wchar_t) == 2) {
        // Windows: UChar와 wchar_t 크기가 같음
        return std::wstring(reinterpret_cast<const wchar_t *>(str.getBuffer()), str.length());
    } else {
        // 유닉스: UChar32로 변환 후 wchar_t에 복사
        int32_t length = str.length();
        std::vector<UChar32> utf32Buffer(length);

        UErrorCode errorCode = U_ZERO_ERROR;
        int32_t destLength = str.toUTF32(utf32Buffer.data(), length, errorCode);

        if (U_FAILURE(errorCode)) {
            return std::wstring();
        }

        // 유닉스에서는 wchar_t가 UChar32와 같은 크기이므로 직접 변환 가능
        return std::wstring(reinterpret_cast<const wchar_t *>(utf32Buffer.data()), destLength);
    }
}
template <> inline bool value_of<bool>(const String &str) {
    if (str == "true") {
        return true;
    } else if (str == "false") {
        return false;
    } else {
        assert(false && "Invalid bool value");
        return false;
    }
}
template <> inline boost::uuids::uuid value_of<boost::uuids::uuid>(const String &str) {
    boost::uuids::string_generator gen;
    try {
        return gen(str.to<std::string>());
    } catch (const std::runtime_error &) {
        return boost::uuids::nil_uuid();
    }
}
template <typename T>
    requires std::is_enum_v<T>
inline T value_of(const String &str) {
    return magic_enum::enum_cast<T>(str.to<std::string>());
}

} // namespace nugdev::ndk

namespace std {
template <> struct hash<nugdev::ndk::String> {
    size_t operator()(const nugdev::ndk::String &str) const {
        // UTF-8로 변환하여 std::string의 해시 함수를 재사용
        std::string utf8;
        str.toUTF8String(utf8);
        return std::hash<std::string>{}(utf8);
    }
};
} // namespace std