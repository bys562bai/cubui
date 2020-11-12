#pragma once
#include <cubui/util/logging.h>
#include "confignode.h"

namespace cubui{
    struct LoggingConfig:ConfigNode{
        virtual const char* getName() const {
            static auto name = "Logging";
            return name;
        }

        virtual Result init(){
            using namespace std;
            using namespace spdlog;
            using namespace global_val;
            
            g_logger = std::make_shared<spdlog::logger>("cubui");
            auto rotatingSink = make_shared<sinks::rotating_file_sink_mt> ("cubuiLog/log.txt", 1024*1024, 1000, false);
            g_logger->sinks().push_back(rotatingSink);

            #ifdef CUBUI_USING_MSVC
            auto msvcSink = make_shared<sinks::msvc_sink_mt>();
            g_logger->sinks().push_back(msvcSink);
            #endif

            g_logger->set_level(level::info);

            
            return 0;
        }
    };


}