#include "loggingconfig.h"


namespace cubui{
    namespace logging{
        using namespace spdlog;
    }

    namespace global_val{
        std::shared_ptr<spdlog::logger> g_logger;
    }


}