#include "logger.h"
// Includes the Logger class declaration and LogLevel enum from the same directory.
// All method definitions below provide the implementations declared in logger.h.

// blank line: separates the include from the namespace block.
namespace DME {
// Re-enters the DME namespace so these definitions are associated with DME::Logger.

// blank line: separates namespace opening from the first function definition.
// Purpose: implements Meyers' singleton — returns the one Logger instance for the process.
Logger& Logger::instance() {
// Definition of the static factory method declared in logger.h.
    static Logger inst;
    // Function-local static: constructed exactly once on the first call; thread-safe under C++11.
    // All subsequent calls skip construction and return the same already-created object.
    return inst;
    // Returns a reference (not a copy) so all callers share the same Logger state.
}
// Closes Logger::instance().

// blank line: separates instance() from set_level().
// Purpose: allows callers to change the minimum logging severity at runtime.
void Logger::set_level(LogLevel level) {
// Definition of the level-setting method declared in logger.h.
    std::lock_guard<std::mutex> lock(mtx_);
    // RAII mutex lock: acquires mtx_ on construction, releases it when lock goes out of scope.
    // Ensures the level_ write is visible to other threads without a data race.
    level_ = level;
    // Stores the new minimum log level; messages below this threshold will be silently dropped.
}
// Closes set_level(); the lock_guard releases mtx_ automatically here.

// blank line: separates set_level() from enable_file().
// Purpose: opens a log file for append writing; future log calls mirror output to it.
void Logger::enable_file(const std::string& filename) {
// Definition of the file-enable method declared in logger.h.
    std::lock_guard<std::mutex> lock(mtx_);
    // Acquires mtx_ to prevent concurrent calls from racing to open different files simultaneously.
    file_.open(filename, std::ios::app);
    // Opens the named file in append mode (std::ios::app) so existing content is preserved.
    // If the file does not exist it will be created; file_ stays open until Logger is destroyed.
}
// Closes enable_file(); the lock_guard releases mtx_ automatically here.

// blank line: separates enable_file() from the thin forwarding wrappers.
void Logger::debug(const std::string& msg) { log(LogLevel::DEBUG, msg); }
// Thin wrapper: forwards the message to log() with the DEBUG severity tag.
void Logger::info (const std::string& msg) { log(LogLevel::INFO,  msg); }
// Thin wrapper: forwards the message to log() with the INFO severity tag.
void Logger::warn (const std::string& msg) { log(LogLevel::WARN,  msg); }
// Thin wrapper: forwards the message to log() with the WARN severity tag.
void Logger::error(const std::string& msg) { log(LogLevel::ERROR, msg); }
// Thin wrapper: forwards the message to log() with the ERROR severity tag.

// blank line: separates the forwarding wrappers from the core log() implementation.
// Purpose: central dispatch — applies level filter, formats the entry, and writes to all sinks.
void Logger::log(LogLevel level, const std::string& msg) {
// Core private method called by debug/info/warn/error; not directly exposed to callers.
    if (level < level_) return;
    // Guard clause: if the message severity is below the configured threshold, discard immediately.
    // Comparing enum class values works because DEBUG(0) < INFO(1) < WARN(2) < ERROR(3).
    std::lock_guard<std::mutex> lock(mtx_);
    // Acquires mtx_ to serialize console and file writes across concurrent threads.
    // Placed AFTER the level check to avoid locking overhead for filtered-out messages.
    std::string line = "[" + timestamp() + "] [" + level_str(level) + "] " + msg;
    // Builds the formatted log line: "[HH:MM:SS] [LEVEL] message text".
    // timestamp() and level_str() capture the exact severity label and time of the call.
    std::cout << line << "\n";
    // Writes the formatted line to standard output (the console).
    // "\n" instead of std::endl avoids the implicit flush overhead on every log call.
    if (file_.is_open()) file_ << line << "\n";
    // If enable_file() was called, mirrors the same line to the open log file.
    // is_open() check guards against writing to a stream that was never opened.
}
// Closes Logger::log(); the lock_guard releases mtx_ automatically here.

// blank line: separates log() from level_str().
// Purpose: converts a LogLevel enum value to a fixed-width string for log-line formatting.
std::string Logger::level_str(LogLevel level) {
// Helper called by log() to embed a human-readable severity label in each log line.
    switch (level) {
    // Switch on the scoped enum value to select the matching fixed-width label string.
        case LogLevel::DEBUG: return "DEBUG";
        // Returns the 5-character label for DEBUG-level messages.
        case LogLevel::INFO:  return "INFO ";
        // Returns "INFO " with a trailing space to column-align with "DEBUG" and "ERROR".
        case LogLevel::WARN:  return "WARN ";
        // Returns "WARN " with a trailing space to maintain consistent column width.
        case LogLevel::ERROR: return "ERROR";
        // Returns the 5-character label for ERROR-level messages.
        default:              return "?????";
        // Fallback for any future enum values not yet handled; signals an unexpected input.
    }
    // Closes the switch block; every case returns, so no code can fall through here.
}
// Closes Logger::level_str().

// blank line: separates level_str() from timestamp().
// Purpose: produces a wall-clock timestamp string ("HH:MM:SS") for each log entry.
std::string Logger::timestamp() {
// Helper called by log() to capture the current local time at the moment of each write.
    std::time_t t = std::time(nullptr);
    // Calls the C standard library to get the current calendar time as seconds since the epoch.
    // nullptr argument means the result is not stored externally; only the return value is used.
    char buf[20];
    // Stack-allocated buffer large enough for "HH:MM:SS\0" (9 bytes) plus a safe margin.
    std::strftime(buf, sizeof(buf), "%H:%M:%S", std::localtime(&t));
    // Formats the time_t value into "HH:MM:SS" using the local timezone; writes result into buf.
    // sizeof(buf) prevents overflow; std::localtime converts epoch seconds to local time fields.
    return std::string(buf);
    // Wraps the C-style char array in a std::string and returns it to the caller.
}
// Closes Logger::timestamp().

// blank line: separates timestamp() from the closing namespace brace.
} // namespace DME
// Closes the DME namespace block; the trailing comment identifies the namespace for readability.
