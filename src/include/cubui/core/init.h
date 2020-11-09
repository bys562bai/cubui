#pragma once
#include <cubui/util/logging.h>
#include <cubui/platform/sdl/core/init.h>

#include <cubui/container/list.h>
#include "usefultype.h"

namespace cubui{

    struct Initializer{
        Initializer(){
            m_nodes.push_back(logNode);
            m_nodes.push_back(sdlNode);
        }

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

        LOGInitNode logNode;
        SDLInitNode sdlNode;
        
        private:
        IntrusiveList<InitNode> m_nodes;
    };
    
    namespace global_val{
        extern int g_argc;
        extern char** g_argv;
        extern Initializer g_init;
    }

}