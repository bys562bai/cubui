#pragma once
#include <cubui/container/list.h>
#include <cubui/core/usefultype.h>

namespace cubui{
    struct InitNode : ListNode{
        InitNode* initThisAndAfter(){
            auto n = this;
            do{
                if(n->inited())
                    continue;
                auto re = n.init();
                if(re){
                    if(n->whenFail(re)){
                        return n;
                    }
                }
                n->m_inited = true;
                n = n->next();
            }while(n);
        }

        virtual bool whenFail(Result re){
            return 1;
        }

        virtual bool inited() {return m_inited;}
        virtual Result init() {return 0;}
        virtual ~InitNode(){}

        friend struct Initializer;
        private:
        bool m_inited = false;
    };
    
    struct Initializer{

        virtual void add(InitNode* n) {
            m_nodes.push_back(*n);
        }

        virtual Result init(int argc, char** argv){

            for(auto& n: m_nodes){
                if(n.inited())
                    continue;
                auto re = n.init(*this);
                if(re) return re;
                n.m_inited = true;
            }
            return 0;
        }

        virtual ~Initializer() {}
        
        private:
        int m_argc = 0;
        char** m_argv = nullptr;
        IntrusiveList<InitNode> m_nodes;
    };
}