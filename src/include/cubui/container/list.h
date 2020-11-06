#pragma once
#include <boost/intrusive/list.hpp>

#define BI boost::intrusive

namespace cubui{
    template<typename T>
    struct IntrusiveList : BI::list{

    };
}

#undef BI