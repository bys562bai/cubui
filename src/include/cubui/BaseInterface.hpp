#pragma once
#include <cubui/common/typedef.hpp>
#include <cubui/template/typebuild.hpp>
#include <cubui/common/typebase.hpp>

namespace cubui{

struct Message;
struct Scene;
struct SceneNode;
struct Renderer;
struct RenderOutput;

struct MsgHandler : TypeBase{
    virtual ~MsgHandler(){}
    virtual bool support_msg(MsgID id){return false;}
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
    virtual Result init(Renderer& renderer);
    // virtual void init(Renderer& renderer, bool multi_renderer = false) = 0;
    virtual Result reinit(Renderer& renderer) = 0;
    // virtual void reinit(Renderer& renderer, bool multi_renderer = false) = 0;
    virtual bool inited() = 0;
    virtual void uninit() = 0;
    virtual Renderer& get_renderer() = 0;

    virtual void draw(Renderer& renderer) = 0;
    // virtual void draw(Renderer& renderer, RenderOutput& out) = 0;
    virtual bool has_son() = 0;
    virtual SceneNodeIterator sons_begin() = 0;
    virtual SceneNodeIterator sons_end() = 0;
};

//desc, info, config
struct Scene2DNodeInfo{
    //边界位置，高度
    //输入边界是否为0，是否无界
    //输出边界是否为0，是否无界
};

struct Scene2DNode : SceneNode{
    virtual void getObject2DInfo(Scene2DNodeInfo& info) = 0;
};

struct Scene2D : Scene{

};

struct Scene3DNode : SceneNode{

};

struct Scene3D : Scene{

};

}