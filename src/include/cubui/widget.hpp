#pragma once
#include <cubui/common/typedef.hpp>
#include <cubui/common/typebuild.hpp>
#include <cubui/common/typebase.hpp>
#include <cubui/common/smartptr.hpp>

namespace cubui{

struct Message;
struct SceneNode;
struct Renderer;

struct MsgHandler : TypeBase{
    virtual ~MsgHandler(){}
    virtual void init(MsgHandler& prnt, Renderer& renderer){}
    virtual void reinit(MsgHandler& prnt, Renderer& renderer){}
    virtual void uninit(MsgHandler& prnt) {}
    virtual bool if_inited(){return true; }
    virtual bool if_support_msg(MsgID id){return false;}
    protected:
    virtual void when(MsgHandler& prnt, MsgHandler& sender, Message& msg){}
};


struct SceneNode : MsgHandler{
    virtual void destroy() = 0;
};

struct Scene : MsgHandler{
    //自带引用计数
};

//desc, info, config
struct Object2DInfo{
    //边界位置，高度
    //输入边界是否为0，是否无界
    //输出边界是否为0，是否无界
};

struct Object2D : SceneNode{
    virtual void getObject2DInfo(Object2DInfo& info);
};


}