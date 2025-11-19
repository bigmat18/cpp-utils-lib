#pragma once

#include <iostream>
#include <string>
#include <mutex>
#include <thread>
#include <sstream>
#include <source_location>

#ifdef _OPENMP
    #include <omp.h>
#endif

#if __has_include(<stacktrace>) 
    #include <stacktrace>
#else
    #pragma message("<stacktrace> not available — stack dumps will be disabled")
#endif

class Assert {

public:
    static inline void Check(bool condition, 
                             const std::string expr, 
                             const std::string& message, 
                             [[maybe_unused]] const std::source_location& location) 
    {
        if (!condition) {
            std::lock_guard<std::mutex> lock(sMutex);
            std::cerr << "\n\x1b[31m[ASSERTION]\x1b[0m\n"; 

            std::cerr << "\t[Information]\n"; 
            std::cerr << "\t\tCondition: "<< expr  << "\n";
            std::cerr << "\t\tThread ID: " << GetThreadID() << "\n";
            std::cerr << "\t\tMsg: " << message << "\n\n";

            std::cerr << "\t[Stacktrace]\n";

            #if __has_include(<stacktrace>) 
                auto trace = std::stacktrace::current();
                for (std::size_t i = 1; i < trace.size(); ++i) {
                    std::cerr << "\t\t" << trace[i] << '\n';
                }
            #else
                std::cerr << "\t\tFile: " << location.file_name() << ":" << location.line() << ":" << location.column() << "\n";
                std::cerr << "\t\tFunction: " << location.function_name() << std::endl;
            #endif

            throw std::runtime_error(message);
        }
    }

private:
    static inline std::mutex sMutex;

    static std::string GetThreadID() {
        #ifdef _OPENMP
        if (omp_in_parallel())
            return std::to_string(omp_get_thread_num());
        #endif
        std::ostringstream oss;
        oss << std::this_thread::get_id();
        return oss.str();
    }
};

#ifndef NDEBUG
    #ifdef ENABLE_ASSERT
        #define massert(condition, message) \
            ::Assert::Check((condition), #condition, message, std::source_location::current())
    #else
        #pragma message("<massert> not availble - custom assert will be disabled")
        #include <cassert>
        #define massert(condition, message) \
            assert((condition) && message)
        #endif
#else
    #define massert(condition, message) ((void)0)
#endif

