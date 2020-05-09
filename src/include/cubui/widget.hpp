#pragma once
#include <cubui/common/typedef.hpp>
#include <cubui/common/typebuild.hpp>
#include <cubui/common/typebase.hpp>
#include <cubui/common/smartptr.hpp>

namespace cubui{

struct Message;
struct Scene;
struct SceneNode;
struct Renderer;

struct MsgHandler : TypeBase{
    virtual ~MsgHandler(){}
    virtual bool if_support_msg(MsgID id){return false;}
    protected:
    virtual void when(MsgHandler* prnt, MsgHandler* sender, Message* msg){}
};


struct SceneNode : MsgHandler{

};

struct Scene : MsgHandler{
    struct Pos; //TODO
    //自带引用计数
    virtual void destroy(SceneNode* node) = 0;
    virtual void move(SceneNode* node, Pos& pos) = 0;
};

//desc, info, config
struct Object2DInfo{
    //边界位置，高度
    //输入边界是否为0，是否无界
    //输出边界是否为0，是否无界
};

struct Object2D : SceneNode{
    virtual void getObject2DInfo(Object2DInfo& info) = 0;
};

struct Scene2D : Scene{

};


}