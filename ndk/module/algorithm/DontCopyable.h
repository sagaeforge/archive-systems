#pragma once

namespace nugdev::ndk {

class DontCopyable {
  public:
    DontCopyable(const DontCopyable &) = delete;
    DontCopyable &operator=(const DontCopyable &) = delete;
};

} // namespace nugdev::ndk