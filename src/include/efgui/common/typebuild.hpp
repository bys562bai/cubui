#pragma once

namespace typebuild{
    template <typename T>
    struct Iterator;

    template<typename T>
    struct IForwardIterator{
        using This =  IForwardIterator<T>;
        virtual T& operator* () const = 0;
        virtual This& operator++() = 0;
        virtual ~IForwardIterator () {}
    };
}
