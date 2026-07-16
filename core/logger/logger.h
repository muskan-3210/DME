#pragma once
// Include guard: ensures this header is processed only once per translation unit.
// Without it, multiple includes would cause redeclaration errors for LogLevel and Logger.
#include <string>
// Provides std::string, used for message parameters and the log filename.
#include <iostream>
// Provides std::cout, used in logger.cpp to print log lines to the console.
#include <fstream>
// Provides std::ofstream for writing log output to a file when file logging is enabled.
#include <mutex>
// Provides std::mutex and std::lock_guard for thread-safe log writing across threads.
#include <ctime>
// Provides std::time, std::localtime, and std::strftime for generating HH:MM:SS timestamps.

// blank line: separates includes from the namespace block.
namespace DME {
// Opens the DME project namespace; Logger and LogLevel are accessed as DME::Logger, DME::LogLevel.

// blank line: separates namespace opening from the enum declaration.
enum class LogLevel { DEBUG, INFO, WARN, ERROR };
// Strongly-typed enum defining the four severity levels in ascending order of urgency.
// enum class (scoped enum) prevents implicit int conversion and name collisions; use as LogLevel::INFO.

// blank line: separates the enum from the class declaration.
class Logger {
// Singleton logger class: one shared instance for the entire DME engine.
// Wraps console and file output behind a thread-safe, level-filtered interface.
public:
    // Access specifier: all declarations below are publicly callable by any code.
    static Logger& instance();
    // Returns the single Logger instance (Meyers' singleton pattern).
    // static member function: called as Logger::instance() with no object required.

    // blank line: separates the singleton accessor from the configuration methods.
    void set_level(LogLevel level);
    // Sets the minimum severity threshold; messages below this level are silently dropped.
    void enable_file(const std::string& filename);
    // Opens the named file in append mode; subsequent log calls also write to this file.

    // blank line: separates configuration methods from the public logging methods.
    void debug(const std::string& msg);
    // Logs a DEBUG-level message; use for detailed diagnostic output during development.
    void info (const std::string& msg);
    // Logs an INFO-level message; use for normal operational events.
    void warn (const std::string& msg);
    // Logs a WARN-level message; use for recoverable unexpected conditions.
    void error(const std::string& msg);
    // Logs an ERROR-level message; use for failures that need immediate attention.

    // blank line: separates the public interface from private implementation details.
private:
    // Access specifier: everything below is hidden from callers — singleton internals only.
    Logger() = default;
    // Default constructor is private to prevent direct instantiation; only instance() creates a Logger.
    // = default: asks the compiler to generate the trivial default constructor body automatically.
    void log(LogLevel level, const std::string& msg);
    // Core internal dispatch: applies level filtering, formats the entry, and writes to all sinks.
    std::string level_str(LogLevel level);
    // Converts a LogLevel enum value to a fixed-width human-readable string (e.g., "WARN ").
    std::string timestamp();
    // Returns the current wall-clock time as a formatted "HH:MM:SS" string.

    // blank line: separates private method declarations from private member variables.
    LogLevel     level_  = LogLevel::INFO;
    // Current minimum log level; messages strictly below this threshold are filtered out.
    // Trailing underscore is a common C++ convention for private member variables.
    std::ofstream file_;
    // Output file stream; valid and open only after enable_file() has been called.
    // std::ofstream manages its own lifetime — closes the file automatically in its destructor.
    std::mutex   mtx_;
    // Mutex protecting level_, file_, and console output from concurrent access.
    // Ensures log lines from multiple threads are not interleaved or corrupted.
};
// Closes the Logger class definition.

// blank line: separates the class definition from the convenience macro definitions below.
#define LOG_INFO(msg)  DME::Logger::instance().info(msg)
// Convenience macro: expands to a Logger::info() call with the given message expression.
// Fully qualified DME:: so the macro works correctly even outside the DME namespace.
#define LOG_WARN(msg)  DME::Logger::instance().warn(msg)
// Convenience macro for WARN-level logging; same pattern as LOG_INFO.
#define LOG_ERROR(msg) DME::Logger::instance().error(msg)
// Convenience macro for ERROR-level logging; use at error-handling sites throughout the engine.
#define LOG_DEBUG(msg) DME::Logger::instance().debug(msg)
// Convenience macro for DEBUG-level logging; typically filtered out in production configurations.

// blank line: separates the macros from the closing namespace comment.
} // namespace DME
// Closes the DME namespace; the trailing comment names it for readability in large files.
