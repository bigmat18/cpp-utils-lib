#pragma once

#include "massert.hpp"
#include <algorithm>
#include <cstdio>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <mutex>
#include <sstream>
#include <string>
#include <chrono>

inline std::string __GetFileName(const char* path) {
    const char* file = path;
    for (const char* p = path; *p; ++p) {
        if (*p == '/' || *p == '\\') file = p + 1;
    }
    return std::string(file);
}

inline std::string __GetCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    char timestamp_buffer[64];
    std::strftime(timestamp_buffer, sizeof(timestamp_buffer),
                  "%Y-%m-%d %X", std::localtime(&in_time_t));
    return timestamp_buffer;
}

namespace fs = std::filesystem;

class Logging {

    static inline std::mutex s_Mutex;
    static inline bool s_IsInit = false;

    std::ostringstream m_Buffer; 
    fs::path m_Filename = "file.log";
    fs::path m_Dir = fs::current_path() / "logs"; 

    static Logging& instance() {
        static Logging loggingSystem;
        return loggingSystem;
    }

public:
    static void initialize(fs::path dir = fs::current_path() / "logs")
    {
        std::lock_guard<std::mutex> lock(s_Mutex);
        if (!s_IsInit) {
            std::string time = __GetCurrentTimestamp();
            std::replace_if(time.begin(), time.end(), 
                            [](unsigned char c) { return c == ' '; }, 
                            '_');

            instance().m_Filename = time + ".log";
            instance().m_Dir = dir;
            s_IsInit = true;
        }
    }

    static void shutdown() {
        if (s_IsInit) {
            if (!fs::exists(instance().m_Dir)) {
                fs::create_directories(instance().m_Dir);
            }

            fs::path fullPath = instance().m_Dir / instance().m_Filename;
            std::ofstream file(fullPath);
            massert(file.is_open(), "Error during opens of " + fullPath.string());

            file << instance().m_Buffer.str();
            file.close();
            instance().m_Buffer.clear();
            s_IsInit = false;
        }
    }

    static void write(std::string msg) { 
        std::lock_guard<std::mutex> lock(s_Mutex);
        if (s_IsInit)
            instance().m_Buffer << msg; 
    }

    Logging() = default;

    ~Logging() = default;

    Logging(const Logging& other) = delete;

    Logging(Logging&& other) = delete;

    Logging& operator=(const Logging&) = delete;
};

#if !defined (_WIN32)
    #define RED     "\x1b[31m"
    #define YELLOW  "\x1b[33m"
    #define GREEN   "\x1b[32m"
    #define BLUE    "\x1b[36m"
    #define RESET   "\x1b[0m"
#else
    #define RED     ""
    #define YELLOW  ""
    #define GREEN   ""
    #define BLUE    ""
    #define RESET   ""
#endif

#ifdef ENABLE_LOGGING

#if HAS_STD_FORMAT

    #define __INTERNAL(level_str, color, fmt, ...)                                  \
        do {                                                                        \
            std::string time = __GetCurrentTimestamp();                             \
            std::string filename = __GetFileName(__FILE__);                         \
            int line = __LINE__;                                                    \
            std::string prefix = std::format(                                       \
                "{}[{}][{}:{}][{}] ",                                               \
                color, time, filename, line, level_str                              \
            );                                                                      \
            std::string content = std::format(fmt __VA_OPT__(, __VA_ARGS__));       \
            auto msg = prefix + content + "\n" + RESET;                             \
            std::cout << msg;                                                       \
            Logging::write(msg);                                                    \
        } while (0)

#else

    #define __INTERNAL(level_str, color, fmt, ...)                                  \
        do {                                                                        \
            std::string time = __GetCurrentTimestamp();                             \
            std::string filename = __GetFileName(__FILE__);                         \
            int line = __LINE__;                                                    \
            std::string prefix = "[" + time + "][" + filename + ":"                 \
                               + std::to_string(line) + "][" + level_str + "] ";    \
            std::string content = fmt;                                              \
            std::string warn = std::string(YELLOW) + "[std::format not available]"  \
                               + std::string(RESET);                                \
            auto msg = color + prefix + warn + content + "\n" + RESET;              \
            std::cout << msg;                                                       \
            Logging::write(msg);                                                    \
        } while (0)

#endif
#else
    #define __INTERNAL(level_str, color, fmt, ...) (void(0))
#endif // ENABLE_LOGGING

#define LOG_ERROR(fmt, ...) __INTERNAL("ERROR", RED,    fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...)  __INTERNAL("WARN",  YELLOW, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...)  __INTERNAL("INFO",  GREEN,  fmt, ##__VA_ARGS__)

#ifndef NDEBUG
    #define LOG_DEBUG(fmt, ...) __INTERNAL("DEBUG", BLUE, fmt, ##__VA_ARGS__)
#else
    #define LOG_DEBUG(fmt, ...) (void(0))
#endif
