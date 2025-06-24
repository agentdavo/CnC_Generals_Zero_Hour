#include "common/logger.h"

#include <cstdarg>
#include <cstdio>
#include <fstream>
#include <iostream>

static std::ofstream logFile;
static bool fileOpen = false;

void Logger::init(const std::string &file)
{
    if (!file.empty()) {
        logFile.open(file.c_str(), std::ios::out | std::ios::trunc);
        if (logFile.is_open())
            fileOpen = true;
    }
}

void Logger::shutdown()
{
    if (logFile.is_open()) {
        logFile.flush();
        logFile.close();
    }
    fileOpen = false;
}

void Logger::log(LogLevel level, const char *fmt, ...)
{
    char buf[1024];
    va_list args;
    va_start(args, fmt);
    std::vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);

    const char *prefix = "";
    switch (level) {
    case LogLevel::Info:
        prefix = "[INFO] ";
        break;
    case LogLevel::Warn:
        prefix = "[WARN] ";
        break;
    case LogLevel::Error:
        prefix = "[ERROR] ";
        break;
    }

    std::clog << prefix << buf << std::endl;
    if (fileOpen)
        logFile << prefix << buf << std::endl;
}

