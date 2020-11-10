#pragma once
#include "predef.h"

#ifdef CUBUI_PLATFORM_WINDOWS

#endif

#include <cubui/util/confignode.h>
#include <cubui/util/logging.h>

#include <cubui/common/usefultype.h>
#include <cubui/common/global.h>

namespace cubui
{
    struct CubuiConfig {
        ConfigNode* init(int argc, char** argv) {
            {
                using namespace global_val;
                g_argc = argc;
                g_argv = argv;
            }

            ConfigNode* arr[] = { 
                &logConfig
                };

            for (int i = 0; i < sizeof(arr)/sizeof(ConfigNode*); i++) {
                auto n = arr[i];
                if (n->inited())
                    continue;
                if (auto re = n->init()) {
                    if(n->whenFail(re))
                        return n;
                }
                n->set_inited();
            }
            return nullptr;
        }

        LOGConfig logConfig;
    };
    
} // namespace cubui
