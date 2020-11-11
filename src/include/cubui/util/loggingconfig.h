#pragma once
#include <memory>
#include <string>
#include <filesystem>

#include <cubui/common/predef.h>

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_INFO
#include <spdlog/spdlog.h>

#ifdef CUBUI_USING_MSVC
#include <spdlog/sinks/msvc_sink.h>
#endif

#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>

#include <cubui/util/confignode.h>

namespace cubui{
    namespace global_val{
        
       extern std::shared_ptr<spdlog::logger> g_logger;
    }

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