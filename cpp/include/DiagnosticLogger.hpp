#pragma once

#include <chrono>
#include <iostream>
#include <string_view>

namespace xau {

enum class LogLevel {
    Debug,
    Info,
    Warning,
    Error
};

class DiagnosticLogger {
public:
    static void log(
        const LogLevel level,
        const std::string_view event,
        const std::string_view detail = {}
    ) {
        const auto now = std::chrono::system_clock::now();
        const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()
        ).count();

        std::cout
            << "[" << ms << "] "
            << levelName(level)
            << " EVENT=" << event;

        if (!detail.empty()) {
            std::cout << " " << detail;
        }

        std::cout << '\n';
    }

private:
    static constexpr std::string_view levelName(const LogLevel level) noexcept {
        switch (level) {
            case LogLevel::Debug: return "DEBUG";
            case LogLevel::Info: return "INFO";
            case LogLevel::Warning: return "WARN";
            case LogLevel::Error: return "ERROR";
        }

        return "UNKNOWN";
    }
};

} // namespace xau
