#pragma once
#include <cubui/common/usefultype.h>

namespace cubui{
    struct ConfigNode{
        virtual bool whenFail(Result re){
            return 1;
        }

        virtual bool inited() {return m_inited;}
        virtual Result init() {return 0;}
        virtual ~ConfigNode(){}

        void set_inited(bool x = true) { m_inited = x; }
        private:
        bool m_inited = false;
    };
}