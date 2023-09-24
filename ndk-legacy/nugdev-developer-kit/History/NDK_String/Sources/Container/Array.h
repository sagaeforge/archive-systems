//
// Created by Jinwon Choi on 9/18/23.
//

#pragma once

namespace ndk::lib::container {

    class IArray {};

    template <typename T, unsigned int Size>
    class StaticArray : public IArray {
    public:
        class Iterator {
        public:
            Iterator(T *ptr) : _ptr(ptr) {}

            Iterator operator++() {
                _ptr++;
                return *this;
            }

            Iterator operator++(int) {
                Iterator i = *this;
                _ptr++;
                return i;
            }

            bool operator==(const Iterator &rhs) const { return _ptr == rhs._ptr; }

            bool operator!=(const Iterator &rhs) const { return _ptr != rhs._ptr; }

            T &operator*() { return *_ptr; }

        private:
            T *_ptr;
        };

    private:
        T _preBuffer;
        T _array[Size];
        T _postBuffer;
    };

}; // namespace ndk::lib::container