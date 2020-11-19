#pragma once
#include <boost/intrusive/list.hpp>

#define BI boost::intrusive

namespace cubui{
    namespace _inner_local{
        using namespace boost::intrusive;

        struct IntruListNode: list_base_hook<link_mode<auto_unlink> >{
            using _BASE = list_base_hook<link_mode<auto_unlink> >;
            using _BASE::_BASE;
        };

        template<typename T>
        using IntrusiveList = list<T, constant_time_size<false> >;
    }

    using _inner_local::IntruListNode;
    using _inner_local::IntrusiveList;
}

#undef BI