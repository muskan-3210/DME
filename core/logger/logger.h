#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <mutex>
#include <ctime>

namespace DME {

enum class LogLevel { DEBUG, INFO, WARN, ERROR };

class Logger {
public:
    static Logger& instance();

    void set_level(LogLevel level);
    void enable_file(const std::string& filename);

    void debug(const std::string& msg);
    void info (const std::string& msg);
    void warn (const std::string& msg);
    void error(const std::string& msg);

private:
    Logger() = default;
    void log(LogLevel level, const std::string& msg);
    std::string level_str(LogLevel level);
    std::string timestamp();

    LogLevel     level_  = LogLevel::INFO;
    std::ofstream file_;
    std::mutex   mtx_;
};

#define LOG_INFO(msg)  DME::Logger::instance().info(msg)
#define LOG_WARN(msg)  DME::Logger::instance().warn(msg)
#define LOG_ERROR(msg) DME::Logger::instance().error(msg)
#define LOG_DEBUG(msg) DME::Logger::instance().debug(msg)

} // namespace DME
