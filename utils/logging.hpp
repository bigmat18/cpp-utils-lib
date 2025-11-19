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

#ifdef ENABLE_LOGGING
    #ifndef NDEBUG
        #define __INTERNAL(level_str, color, format, ...)                   \
            do {                                                            \
                std::string time = __GetCurrentTimestamp();                 \
                std::string filename = __GetFileName(__FILE__);             \
                int line = __LINE__;                                        \
                auto msg = std::vformat(                                    \
                    color + std::string("[{}][{}:{}][{}] ") + format + "\n",\
                    std::make_format_args(                                  \
                        time, filename, line,                               \
                        level_str, ##__VA_ARGS__)) + "\x1b[0m";             \
                std::cout << msg;                                           \
                Logging::write(msg);                                        \
            } while (0)
    #else 
        #define __INTERNAL(level_str, color, format, ...)                   \
            do {                                                            \
                const char* time = __GetCurrentTimestamp().c_str();         \
                auto msg = std::vformat(                                    \
                    color + std::string("[{}][{}:{}][{}] ") + format + "\n",\
                    std::make_format_args(                                  \
                        time, level_str, ##__VA_ARGS__));                   \
                std::cout << msg;                                           \
            } while (0)
    #endif // !NDEBUG
#else 
    #define __INTERNAL(level_str, format, ...) (void(0))
#endif // ENABLE_LOGGING 


#define LOG_ERROR(format, ...) __INTERNAL("ERROR", "\x1b[31m", format, ##__VA_ARGS__)
#define LOG_WARN(format, ...) __INTERNAL("WARN", "\x1b[33m", format, ##__VA_ARGS__)
#define LOG_INFO(format, ...) __INTERNAL("INFO", "\x1b[32m", format, ##__VA_ARGS__)

#ifndef NDEBUG
    #define LOG_DEBUG(format, ...) __INTERNAL("DEBUG", "\x1b[36m", format, ##__VA_ARGS__)
#else 
    #define LOG_DEBUG(format, ...) (void(0))
#endif // !NDEBUG

