#pragma once
// lib/core/logger.h
#include <string>

namespace core {

enum class LogLevel { DEBUG, INFO, WARN, ERROR };

class Logger {
public:
    explicit Logger(const std::string& component) : component_(component) {}
    void Debug(const std::string& msg) const { Log(LogLevel::DEBUG, msg); }
    void Info (const std::string& msg) const { Log(LogLevel::INFO,  msg); }
    void Warn (const std::string& msg) const { Log(LogLevel::WARN,  msg); }
    void Error(const std::string& msg) const { Log(LogLevel::ERROR, msg); }
private:
    void Log(LogLevel level, const std::string& msg) const;
    std::string component_;
};

}  // namespace core
