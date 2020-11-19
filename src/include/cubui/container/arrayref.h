#pragma once
#include <optional>
#include <algorithm>
#include <string.h>
#include <ostream>
#include <stdint.h>

namespace cubui{

    //This class is for convenient and efficient, all details are fully exposed
    template<typename T>
    struct ArrayRef{
        T* data = nullptr;
        size_t size = 0; 

        ArrayRef() = default;
        ArrayRef(T* data, size_t size) {
            this->data = data;
            this->size = size;
        }
        ArrayRef(const ArrayRef<T>& arr) = default;
        ArrayRef(ArrayRef<T>&& arr) = default;

        ArrayRef<T>& operator = (const ArrayRef<T>& arr) = default;
        ArrayRef<T>& operator = (ArrayRef<T>&& arr) = default;

        bool operator == (ArrayRef<T>& arr){
            if(size!= arr.size){
                return false;
            }
            if(size)
                return !memcmp(data, arr.data, size);
            return true;
        }

        T& operator[] (size_t i) { return data[i]; }

        bool empty() {
            return size;
        }

        //assign by bytes
        void fillb(int x) {
            auto bytesize = size * sizeof(T);
            if (bytesize)
                memset(data, x, bytesize);
        }

        void fill(T x) {
            std::fill(begin(), end(), x);
        }

        //return copy number of elements
        size_t copy(const ArrayRef<T>& src) {
            auto cpy_size = std::min(size, src.size);
            if(cpy_size)
                memcpy(data, src.data, sizeof(T) * cpy_size);
            return cpy_size;
        }

        void reset(){
            data = nullptr;
            size = 0;
        }

        void reset(T* data, size_t size) {
            this->data = data;
            this->size = size;
        }

        T* begin() {
            return data;
        }

        T* end() {
            return data + size;
        }

        //left close, right open
        //it will not check if out of range
        ArrayRef<T> slice(size_t a, size_t b) {
            return ArrayRef<T>(data + a, b - a);
        }

        //return idx of matched
        std::optional<size_t> find(const T& v) {
            for (size_t i = 0; i < size; i++) {
                if (data[i] == v) {
                    return i;
                }
            }
        }

        friend std::ostream& operator<< (std::ostream& os, const ArrayRef<T>& arr) {
            auto data = arr.data;
            auto size = arr.size;

            os << '[';
			if (size) {
				auto p = data;
				auto last = p + size  - 1;
				for (;;p++) {
					os << *p;
					if (p < last)
						os << ", ";
                    else 
                        break;
				}
			}
            os << ']';
            return os;
        }
    };
}