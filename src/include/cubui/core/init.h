#pragma once
#include <cubui/container/list.h>
#include "usefultype.h"

namespace cubui{
    struct Initializer;

    namespace global_val{
        extern int g_argc;
        extern char** g_argv;
        extern Initializer g_init;
    }


    struct InitNode : ListNode{
        InitNode();

        virtual bool inited() {return m_inited;}
        virtual Result init(Initializer& initzer) = 0;
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
            using namespace global_val;
            g_argc = argc;
            g_argv = argv;

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
        IntrusiveList<InitNode> m_nodes;
    };
}