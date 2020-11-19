#pragma once
#include <cubui/ext/vulkaninc.h>
#include <cubui/config/confignode.h>

namespace cubui{
    struct CUVulkanConfig : ConfigNode{
        virtual CUResult init(){
            if(glfwInit()){
                return 0;
            }
            return -1;
        }

        virtual void uninit(){
            glfwTerminate();
        }

        virtual ~CUVulkanConfig(){
            uninit();
        }
    };
}