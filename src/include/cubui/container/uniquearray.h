#pragma once
#include <memory>
#include "arrayref.h"

namespace cubui{

    template<typename T, typename Deleter = std::default_delete<T[]> >
    struct UniqueArray {
        std::unique_ptr<T, Deleter> data;
        size_t size = 0;

        UniqueArray() = default;
        UniqueArray(T* data, size_t size) {
            reset(data, size);
        }
        UniqueArray(const UniqueArray<T, Deleter>& arr) = default;
        UniqueArray(UniqueArray<T, Deleter>&& arr) = default;

        UniqueArray<T, Deleter>& operator = (const UniqueArray<T, Deleter>& arr) = default;
        UniqueArray<T, Deleter>& operator = (UniqueArray<T, Deleter>&& arr) = default;
        
        void reset() {
            data = nullptr;
            size = 0;
        }

        void reset(T* data, size_t size) {
            this->data.reset(data);
            this->size = size;
        }

        T* release() {
            return data;
        }

        bool empty() const{
            return size;
        }

        T* begin() {
            return data.get();
        }

        T* end() {
            return data.get() + size;
        }

        T& operator[] (size_t i) { return data.get()[i]; }

        ArrayRef<T> get() {
            return ArrayRef<T>(data.get(), size);
        }

        ArrayRef<T> slice(size_t a, size_t b) {
            return ArrayRef<T>(data.get() + a, b - a);
        }

        size_t copy(const ArrayRef<T>& src) {
            auto cpy_size = std::min(size, src.size);
            if (cpy_size)
                memcpy(data.get(), src.data, sizeof(T) * cpy_size);
            return cpy_size;
        }
    };
}