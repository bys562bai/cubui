#pragma once
#include <cubui/common/typedef.hpp>
#include <cubui/common/typebuild.hpp>
#include <cubui/common/typebase.hpp>
#include <cubui/common/smartptr.hpp>

namespace cubui{

struct Message;
struct WidgetNode;
struct Renderer;

struct WidgetBase : TypeBase{
    virtual ~WidgetBase(){}
    virtual void init(WidgetBase& prnt, Renderer& renderer){}
    virtual void reinit(WidgetBase& prnt, Renderer& renderer){}
    virtual void uninit(WidgetBase& prnt) {}
    virtual bool if_inited(){return true; }
    virtual bool if_support_msg(MsgID id){return false;}
    protected:
    virtual void when(WidgetBase& prnt, WidgetBase& sender, Message& msg){}
};

using WidgetBaseIterator = typebuild::IForwardIterator<WidgetBase>;
using WidgetBaseIteratorPtr = util::unique_ptr<WidgetBaseIterator>;

struct WidgetNode : WidgetBase{
    virtual WidgetNode& root() = 0;
    virtual WidgetNode& prnt() = 0;
    virtual WidgetBaseIteratorPtr sons_iter_begin() = 0;
    virtual WidgetBaseIteratorPtr sons_iter_end() = 0;
    virtual bool has_sons() = 0;
    virtual void destroy() = 0;
};

//desc, info, config
struct WidgetInfo{
    //边界位置，高度
    //输入边界是否为0，是否无界
    //输出边界是否为0，是否无界
};

template<typename T>
struct Point2D;

struct IWidget : WidgetNode{
    virtual void update(WidgetInfo& ) = 0;  //self info
    virtual void move_to(Point2D<int>& pt) = 0; 
};


//Widget是2D的，Spirit是3D的
}