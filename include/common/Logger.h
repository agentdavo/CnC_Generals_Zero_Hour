#pragma once

#include <string>

enum class LogLevel {
    Info,
    Warn,
    Error
};

class Logger {
public:
    static void init(const std::string &file = std::string());
    static void shutdown();
    static void log(LogLevel level, const char *fmt, ...);
};

#define LOG_INFO(fmt, ...)  Logger::log(LogLevel::Info, fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...)  Logger::log(LogLevel::Warn, fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) Logger::log(LogLevel::Error, fmt, ##__VA_ARGS__)

