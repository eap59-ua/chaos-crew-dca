#include "Logger.hpp"
#include <iostream>
#include <vector>

Logger& Logger::GetInstance() {
    static Logger instance;
    return instance;
}

void Logger::Init(const std::string& logFile) {
    try {
        std::vector<spdlog::sink_ptr> sinks;

        // Console sink with colors
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        console_sink->set_level(spdlog::level::debug);
        console_sink->set_pattern("[%^%l%$] %v");
        sinks.push_back(console_sink);

        // File sink (with rotation if needed in the future)
        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFile, true);
        file_sink->set_level(spdlog::level::trace);
        file_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] [%s:%#] %v");
        sinks.push_back(file_sink);

        // Create multi-sink logger
        logger_ = std::make_shared<spdlog::logger>("chaos-crew", sinks.begin(), sinks.end());
        logger_->set_level(spdlog::level::debug);
        logger_->flush_on(spdlog::level::err); // Auto-flush on errors

        // Register logger globally
        spdlog::register_logger(logger_);

        LOG_INFO("=== Logger initialized successfully ===");
        LOG_INFO("Log file: {}", logFile);

    } catch (const spdlog::spdlog_ex& ex) {
        std::cerr << "Logger initialization failed: " << ex.what() << std::endl;
    }
}
