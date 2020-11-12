#pragma once
#include <cubui/common/predef.h>

#include <memory>
#include <string>


#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_INFO
#include <spdlog/spdlog.h>

#ifdef CUBUI_USING_MSVC
#include <spdlog/sinks/msvc_sink.h>
#endif

#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>

namespace cubui{
    namespace global_val{
       extern std::shared_ptr<spdlog::logger> g_logger;
    }
}