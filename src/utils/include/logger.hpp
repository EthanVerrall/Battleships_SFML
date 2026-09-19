#pragma once

// ============================================================================
// Config
// ----------------------------------------------------------------------------

#define ENABLE_LOGS

// ============================================================================
// Includes
// ----------------------------------------------------------------------------

#include <cstdint>
#include <ostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <source_location>
#include <sstream>
#include <string>

// ============================================================================
// Namespaces
// ----------------------------------------------------------------------------

namespace battleships::utils {

// ============================================================================
// Enums
// ----------------------------------------------------------------------------

enum class Log_lvl : std::uint8_t {

    TRACE = 1u << 0u,
    DEBUG = 1u << 1u,
    INFO  = 1u << 2u,
    WARN  = 1u << 3u,
    ERR   = 1u << 4u,
};

inline std::ostream& operator<<(
    std::ostream& os,
    Log_lvl const log_lvl
    ) {

    switch (log_lvl) {

        case Log_lvl::TRACE: return os << "TRACE";
        case Log_lvl::DEBUG: return os << "DEBUG";
        case Log_lvl::INFO:  return os << "INFO";
        case Log_lvl::WARN:  return os << "WARN";
        case Log_lvl::ERR:   return os << "ERROR";

        default: return os << "UNKNOWN LOG LVL";
    }
}

inline std::uint8_t operator|(
    Log_lvl const left,
    Log_lvl const right
    ) {

    return static_cast<std::uint8_t>(left) | static_cast<std::uint8_t>(right);
}

inline std::uint8_t operator&(
    Log_lvl const left,
    Log_lvl const right
    ) {

    return static_cast<std::uint8_t>(left) & static_cast<std::uint8_t>(right);
}

inline std::uint8_t operator|(
    std::uint8_t const left,
    Log_lvl const right
    ) {

    return left | static_cast<std::uint8_t>(right);
}

inline std::uint8_t operator&(
    std::uint8_t const left,
    Log_lvl const right
    ) {

    return left & static_cast<std::uint8_t>(right);
}

inline bool operator==(
    std::uint8_t const left,
    Log_lvl const right
    ) {

    return left == static_cast<std::uint8_t>(right);
}

// ============================================================================
// Class Logger
// ----------------------------------------------------------------------------

class Logger final {

public:
    Logger(Log_lvl const level, std::source_location const loc);

    ~Logger();

    Logger(Logger const&) = delete;
    Logger& operator=(Logger const&) = delete;
    Logger(Logger&&) = delete;
    Logger& operator=(Logger&&) = delete;

    static std::uint8_t ENABLED_LOG_LVLS;

    static bool is_log_lvl_enabled(Log_lvl const log_lvl);

private:
    void _flush();

    char const* _get_color() const;

    std::string _get_file() const;

    std::string _curr_time() const;

    static std::mutex s_mutex;

    Log_lvl _level;
    std::source_location _location;
    std::ostringstream _buffer;

public:
    template<typename Input_type>
    Logger& operator<<(Input_type const& value) {

        _buffer << value;
        return *this;
    }
};

}

// ============================================================================
// Macros
// ----------------------------------------------------------------------------

#ifdef ENABLE_LOGS
    #define LOG(level) \
        if (!battleships::utils::Logger::is_log_lvl_enabled(level)) {} \
        else battleships::utils::Logger(level, std::source_location::current())

    #define LOG_LVLS_ENABLED(level_mask) battleships::utils::Logger::ENABLED_LOG_LVLS = static_cast<std::uint8_t>(level_mask)
#else
    #define LOG(level) if (true) {} else battleships::utils::Logger(level, std::source_location::current())

    #define LOG_LVLS_ENABLED(level_mask)
#endif
