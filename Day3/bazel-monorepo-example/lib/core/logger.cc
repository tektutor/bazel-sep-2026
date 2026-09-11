// lib/core/logger.cc
#include "lib/core/logger.h"
#include <ctime>
#include <iostream>

namespace core {

namespace {
const char* LevelStr(LogLevel l) {
    switch (l) {
        case LogLevel::DEBUG: return "DEBUG";
        case LogLevel::INFO:  return "INFO ";
        case LogLevel::WARN:  return "WARN ";
        case LogLevel::ERROR: return "ERROR";
    }
    return "?????";
}
}  // namespace

void Logger::Log(LogLevel level, const std::string& msg) const {
    std::time_t t = std::time(nullptr);
    char buf[20];
    std::strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%S", std::gmtime(&t));
    std::cerr << buf << " [" << LevelStr(level) << "] "
              << component_ << ": " << msg << "\n";
}

}  // namespace core
// cache demo
