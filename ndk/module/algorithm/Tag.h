#pragma once

#include <boost/uuid.hpp>

#include "string/String.hpp"

namespace nugdev::ndk {

class Tag {
  public:
    Tag();
    Tag(const String &str);
    Tag(const Tag &);
    Tag(Tag &&) noexcept;

  public:
    Tag &operator=(Tag &&) noexcept;
    Tag &operator=(const Tag &) noexcept;
    Tag &operator=(const String &str);
    bool operator==(const Tag &) const noexcept;
    std::strong_ordering operator<=>(const Tag &) const noexcept;

  public:
    std::size_t hash() const;

  private:
    boost::uuids::uuid m_id;
    std::size_t m_hash;
};

} // namespace nugdev::ndk
