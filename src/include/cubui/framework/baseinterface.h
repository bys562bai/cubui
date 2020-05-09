#pragma once
#include <cubui/common/typedef.h>
#include <cubui/template/typebuild.h>
#include <cubui/common/basetype.h>

namespace cubui{

struct Message;
struct IScene;
struct ISceneNode;
struct Renderer;
struct RenderOutput;

struct IMsgHandler : BaseType{
    virtual ~IMsgHandler(){}
    virtual bool support_msg(MsgID id){return false;}
    protected:
    virtual void when(Message* msg, IMsgHandler* prnt, IMsgHandler* sender){}
};

struct IScene : IMsgHandler{
    //应当以shared_ptr方式保存
    virtual Result init(Renderer& renderer) = 0;
    // virtual void init(Renderer& renderer, bool multi_renderer = false) = 0;
    virtual Result reinit(Renderer& renderer) = 0;
    // virtual void reinit(Renderer& renderer, bool multi_renderer = false) = 0;
    virtual bool inited() = 0;
    virtual void uninit() = 0;
    virtual Renderer& get_renderer() = 0;

    virtual void draw(RenderOutput& out) = 0;
    // virtual void draw(Renderer& renderer, RenderOutput& out) = 0;
};

//desc, info, config
struct Object2DInfo{
    //边界位置，高度
    //输入边界是否为0，是否无界
    //输出边界是否为0，是否无界
};

struct IObject2D {
    virtual void getObject2DInfo(Object2DInfo& info) = 0;
};



struct IScene2D : IScene{

};

struct IScene3D : IScene{

};

}