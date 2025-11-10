#pragma once

#include <cstdio>
#include <string>
#include <chrono>

#ifdef ENABLE_LOGGING
    inline const char* GetFileName(const char* path) {
        const char* file = path;
        for (const char* p = path; *p; ++p) {
            if (*p == '/' || *p == '\\') file = p + 1;
        }
        return file;
    }
    
    inline std::string GetCurrentTimestamp() {
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);
        char timestamp_buffer[64];
        std::strftime(timestamp_buffer, sizeof(timestamp_buffer),
                      "%Y-%m-%d %X", std::localtime(&in_time_t));
        return timestamp_buffer;
    }

    #ifndef NDEBUG
        #define __INTERNAL(level_str, format, ...)                          \
        {                                                                   \
            printf("[%s][%s:%d][%s] " format "\n",                          \
                   GetCurrentTimestamp().c_str(),                           \
                   GetFileName(__FILE__), __LINE__,                         \
                   level_str,                                               \
                   ##__VA_ARGS__);                                          \
        }
    #else 
        #define __INTERNAL(level_str, format, ...)                          \
        {                                                                   \
            printf("[%s][%s] " format "\n",                                 \
                   GetCurrentTimestamp().c_str(),                           \
                   level_str,                                               \
                   ##__VA_ARGS__);                                          \
        }
    #endif // !NDEBUG

#else 
    #define __INTERNAL(level_str, format, ...) (void(0))
#endif // ENABLE_LOGGING 


#define LOG_ERROR(format, ...) __INTERNAL("ERROR", format, ##__VA_ARGS__)
#define LOG_WARN(format, ...) __INTERNAL("WARN", format, ##__VA_ARGS__)
#define LOG_INFO(format, ...) __INTERNAL("INFO", format, ##__VA_ARGS__)

#ifndef NDEBUG
    #define LOG_DEBUG(format, ...) __INTERNAL("DEBUG", format, ##__VA_ARGS__)
#else 
    #define LOG_DEBUG(format, ...) (void(0))
#endif // !NDEBUG

