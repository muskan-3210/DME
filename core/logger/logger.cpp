#include "logger.h"

namespace DME {

Logger& Logger::instance() {
    static Logger inst;
    return inst;
}

void Logger::set_level(LogLevel level) {
    std::lock_guard<std::mutex> lock(mtx_);
    level_ = level;
}

void Logger::enable_file(const std::string& filename) {
    std::lock_guard<std::mutex> lock(mtx_);
    file_.open(filename, std::ios::app);
}

void Logger::debug(const std::string& msg) { log(LogLevel::DEBUG, msg); }
void Logger::info (const std::string& msg) { log(LogLevel::INFO,  msg); }
void Logger::warn (const std::string& msg) { log(LogLevel::WARN,  msg); }
void Logger::error(const std::string& msg) { log(LogLevel::ERROR, msg); }

void Logger::log(LogLevel level, const std::string& msg) {
    if (level < level_) return;
    std::lock_guard<std::mutex> lock(mtx_);
    std::string line = "[" + timestamp() + "] [" + level_str(level) + "] " + msg;
    std::cout << line << "\n";
    if (file_.is_open()) file_ << line << "\n";
}

std::string Logger::level_str(LogLevel level) {
    switch (level) {
        case LogLevel::DEBUG: return "DEBUG";
        case LogLevel::INFO:  return "INFO ";
        case LogLevel::WARN:  return "WARN ";
        case LogLevel::ERROR: return "ERROR";
        default:              return "?????";
    }
}

std::string Logger::timestamp() {
    std::time_t t = std::time(nullptr);
    char buf[20];
    std::strftime(buf, sizeof(buf), "%H:%M:%S", std::localtime(&t));
    return std::string(buf);
}

} // namespace DME
