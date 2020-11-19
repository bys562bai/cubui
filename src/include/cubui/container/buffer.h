#pragma once
#include <folly/FBVector.h>
#include "arrayref.h"

namespace cubui{

    template<
        typename T,
        typename Allocator = std::allocator<T> >
        struct CUBuffer : folly::fbvector<T, Allocator> {
        using BASE = folly::fbvector<T, Allocator>;
        using BASE::BASE;

        ArrayRef<T> slice_ref() {
            return ArrayRef<T>(this->data(), this->size());
        }

        ArrayRef<T> slice_ref(size_t a, size_t b) {
            return ArrayRef<T>(this->data(), b-a);
        }

        void del(size_t idx) {
            this->erase(this->begin() + idx);
        }

        template <typename Predicate>
        void erase_if(Predicate predicate) {
            this->erase(std::remove_if(this->begin(), this->end(), predicate), this->end());
        }
    };
    
}