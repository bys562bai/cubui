#pragma once
#include "typedef.hpp"

namespace cubui{

//需要确定用什么容器作为返回值
auto get_type_prnt_ids(TypeID id){
    //TODO
}

struct TypeBase{
    virtual TypeID type_id() {return 0;}
    auto type_prnt_ids(){
        return get_type_prnt_ids(type_id());
    }
};


}