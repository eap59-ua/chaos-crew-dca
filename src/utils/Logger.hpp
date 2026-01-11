#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <memory>
#include <string>

/**
 * @brief Singleton Logger class for centralized logging using spdlog
 *
 * Provides a unified logging interface with multiple sinks:
 * - Console output with color support
 * - File output for persistent logs
 *
 * Usage:
 *   Logger::GetInstance().Init("game.log");
 *   LOG_INFO("Game started");
 *   LOG_DEBUG("Player position: {}, {}", x, y);
 *   LOG_ERROR("Failed to load texture: {}", filename);
 */
class Logger {
public:
    /**
     * @brief Get the singleton instance
     * @return Reference to the Logger instance
     */
    static Logger& GetInstance();

    /**
     * @brief Initialize the logger with console and file sinks
     * @param logFile Path to the log file (default: "chaos-crew.log")
     */
    void Init(const std::string& logFile = "chaos-crew.log");

    /**
     * @brief Log a debug message (detailed information for debugging)
     * @tparam Args Variadic template for format arguments
     * @param args Format string and arguments
     */
    template<typename... Args>
    void Debug(const Args&... args) {
        if (logger_) logger_->debug(args...);
    }

    /**
     * @brief Log an info message (general information)
     * @tparam Args Variadic template for format arguments
     * @param args Format string and arguments
     */
    template<typename... Args>
    void Info(const Args&... args) {
        if (logger_) logger_->info(args...);
    }

    /**
     * @brief Log a warning message (potential issues)
     * @tparam Args Variadic template for format arguments
     * @param args Format string and arguments
     */
    template<typename... Args>
    void Warn(const Args&... args) {
        if (logger_) logger_->warn(args...);
    }

    /**
     * @brief Log an error message (serious problems)
     * @tparam Args Variadic template for format arguments
     * @param args Format string and arguments
     */
    template<typename... Args>
    void Error(const Args&... args) {
        if (logger_) logger_->error(args...);
    }

    /**
     * @brief Log a critical message (system failure)
     * @tparam Args Variadic template for format arguments
     * @param args Format string and arguments
     */
    template<typename... Args>
    void Critical(const Args&... args) {
        if (logger_) logger_->critical(args...);
    }

private:
    Logger() = default;
    ~Logger() = default;

    // Prevent copying
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    std::shared_ptr<spdlog::logger> logger_;
};

// Convenience macros for cleaner code
#define LOG_DEBUG(...) Logger::GetInstance().Debug(__VA_ARGS__)
#define LOG_INFO(...) Logger::GetInstance().Info(__VA_ARGS__)
#define LOG_WARN(...) Logger::GetInstance().Warn(__VA_ARGS__)
#define LOG_ERROR(...) Logger::GetInstance().Error(__VA_ARGS__)
#define LOG_CRITICAL(...) Logger::GetInstance().Critical(__VA_ARGS__)
