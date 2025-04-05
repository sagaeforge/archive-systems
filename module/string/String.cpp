#include "String.hpp"

namespace nugdev::ndk {

String::String(const std::string &str) noexcept : icu::UnicodeString(str.c_str()) {}
String::String(const icu::UnicodeString &other) noexcept : icu::UnicodeString(other) {}

String String::slice(const index_type start, const index_type end) const {
    // 범위 유효성 검사
    if (start > end) {
        return String();
    }

    if (end > this->length()) {
        return this->tempSubString(start, this->length() - start);
    }

    if (start >= this->length()) {
        return String();
    }

    // ICU UnicodeString의 tempSubString 메서드 활용
    return String(this->tempSubString(start, end - start));
}

std::vector<String> String::split(const String &delimiter, const size_type limit) const {
    // 빈 문자열 처리
    if (this->isEmpty()) {
        return {String()};
    }

    // 빈 구분자 처리 (한 글자씩 분할)
    if (delimiter.isEmpty()) {
        std::vector<String> result;
        for (int32_t i = 0; i < this->length(); ++i) {
            UChar32 ch = this->char32At(i);
            result.push_back(String().append(ch));
        }
        return result;
    }

    auto copy = *this;
    std::vector<String> result;
    int count = 0;

    while (limit < 0 || count < limit - 1) {
        auto pos = copy.indexOf(delimiter);
        if (pos == -1) {
            break;
        }

        result.push_back(copy.slice(0, pos));
        copy = copy.slice(pos + delimiter.length(), copy.length());
        count++;
    }

    // 남은 부분 추가
    result.push_back(copy);

    return result;
}
} // namespace nugdev::ndk
