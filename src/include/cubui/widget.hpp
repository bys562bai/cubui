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
struct RenderOutput;

struct MsgHandler : TypeBase{
    virtual ~MsgHandler(){}
    virtual bool if_support_msg(MsgID id){return false;}
    protected:
    virtual void when(MsgHandler* prnt, MsgHandler* sender, Message* msg){}
};


struct SceneNode : MsgHandler{
    //不带引用计数, unique
    virtual Scene* prnt() = 0;
    virtual void destroy() = 0;
};

struct SceneNodeIterator;

struct Scene : MsgHandler{
    //自带引用计数, shared
    virtual void init(Renderer& renderer);
    // virtual void init(Renderer& renderer, bool multi_renderer = false) = 0;
    virtual void reinit(Renderer& renderer) = 0;
    // virtual void reinit(Renderer& renderer, bool multi_renderer = false) = 0;
    virtual void inited() = 0;
    virtual void uninit() = 0;
    virtual Renderer& get_renderer() = 0;

    virtual void draw(Renderer& renderer) = 0;
    // virtual void draw(Renderer& renderer, RenderOutput& out) = 0;
    virtual bool has_son() = 0;
    virtual SceneNodeIterator get_son_it_begin() = 0;
    virtual SceneNodeIterator get_son_it_end() = 0;
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