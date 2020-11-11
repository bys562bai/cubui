#pragma once
#include <cubui/common/usefultype.h>

namespace cubui{
    struct ConfigNode{
        virtual const char* getName() const { 
            static auto name = "UNKNOWN";
            return name; 
        }

        virtual bool whenFail(Result re){
            return 1;   //return 1 means cannot solve the problem
        }

        virtual bool inited() {return m_inited;}

        //return 0 means success, otherwise fail.
        virtual Result init() {return 0;}   
        virtual ~ConfigNode(){}

        void setInited(bool x = true) { m_inited = x; }
        private:
        bool m_inited = false;
    };
}