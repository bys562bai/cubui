#pragma once
#include "logging.h"

#define LOG_TRACE(...) SPDLOG_LOGGER_TRACE(cubui::global_val::g_logger, __VA_ARGS__)
#define LOG_INFO(...) SPDLOG_LOGGER_INFO(cubui::global_val::g_logger, __VA_ARGS__)
#define LOG_WARN(...) SPDLOG_LOGGER_WARN(cubui::global_val::g_logger, __VA_ARGS__)
#define LOG_ERR(...) SPDLOG_LOGGER_ERROR(cubui::global_val::g_logger, __VA_ARGS__)
#define LOG_FATAL(...) SPDLOG_LOGGER_CRITICAL(cubui::global_val::g_logger, __VA_ARGS__)