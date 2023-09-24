#pragma once

#include "MemoryDef.h"

namespace ndk::memory {

// class MemoryLength {
//   using SizeType = unsigned int;

// public:
//   constexpr MemoryLength() : m_value({}) {}
//   constexpr MemoryLength(const SizeType &value) : m_value(value) {}

// private:
//   SizeType m_value;
// };
class MemoryIndex;

// struct RawMemoryUtil {
//   template <typename T> using MemoryPtr = T *;
//   template <typename T> using ConstMemoryPtr = const MemoryPtr<T>;

//   void MemorySetting();
//   void MemoryCopy();
//   void MemoryMove();
//   void MemoryCompare();
//   void MemorySearch();
// };

template <typename T> struct Memory {
  constexpr Memory() {}
  constexpr Memory(T (&value)[])
      : m_ptr{value}, m_length{sizeof(value) / sizeof(T)} {}
  // constexpr Memory(T *value, unsigned int length = 1)

  // constexpr unsigned int GetTypeSize() const { return sizeof(T); }

private:
  T *m_ptr;
  unsigned int m_length;

}; // 임의의 모든 Memory Pointer

struct MemoryUtil {

  // public: // Raw
  //   template <typename T> using RawPtr = T *;
  //   template <typename T> using ConstRawPtr = const RawPtr<T>;
  //   template <typename T> using FinalRawPtr = RawPtr<T> const;
  //   template <typename T> using ConstFinalRawPtr = const FinalRawPtr<T>;

  //   template <typename T> using RawRef = T &;
  //   template <typename T> using ConstRawRef = const RawRef<T>;

  // public: // Any
  //   using AnyPtr = RawPtr<void>;
  //   using ConstAnyPtr = const AnyPtr;
  //   using FinalAnyPtr = AnyPtr const;
  //   using ConstFinalPtr = const FinalAnyPtr;

  //   // template <typename T> using MemoryPtr = Memory<T *>;
  //   // template <typename T> using ConstMemoryPtr = const MemoryPtr<T>;
  //   // template <typename T> using FinalMemoryPtr = MemoryPtr<T> const;
  //   // template <typename T> using ConstFinalMemoryPtr = const
  //   // FinalMemoryPtr<T>;
  //   // template <typename T> using MemoryRef = Memory<T> &;
  //   // template <typename T> using ConstMemoryRef = const MemoryRef<T>;

  // public: // Memory Setting
  //   template <typename T>
  //   static bool MemorySet(AnyPtr address, const T &value, MemoryLength
  //   totalSize); template <typename T> static bool MemorySet(AnyPtr address,
  //   ConstFinalPtr original,
  //                         MemoryLength wordSize, MemoryLength length);

  //   static bool MemorySet(AnyPtr address, ConstFinalPtr original,
  //                         MemoryLength totalSize);
  //   static bool MemorySet(AnyPtr address, ConstFinalPtr original,
  //                         MemoryLength wordSize, MemoryLength length);
  //   template <typename A, typename B>
  //   static bool MemorySet(Memory<A> &address, const Memory<B> &originall,
  //                         MemoryLength length = 0);

  // public: // Memory Move & Copy
  //   static bool MemoryMove();
  //   static bool MemoryCopy();

public: // Memory Compare
        // template <typename A, typename B>
        // bool MemorySet(MemoryRef<A> address, const B &value);
  // template <typename A, typename B>
  // bool MemoryMove(MemoryRef<A> address, ConstMemoryRef<B> orignal,
  //                 bool isCopy = false);

  // template <typename A, typename B>
  // bool MemoryCopy(MemoryRef<A> address, ConstMemoryRef<B> orignal);

  // template <typename T>
  // bool MemoryCompare(ConstMemoryRef<T> address1, ConstMemoryRef<T> address2);
};

} // namespace ndk::memory