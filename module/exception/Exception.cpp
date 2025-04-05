#include "exception/Exception.h"

namespace nugdev::ndk {

Exception::Exception(const String &message, const std::source_location &location) : m_message(message), m_location(location) {
    m_message_str = m_message.to<std::string>();
}

const char *Exception::what() const noexcept { return m_message_str.c_str(); }

} // namespace nugdev::ndk
