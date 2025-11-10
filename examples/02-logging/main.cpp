#include "logging.hpp"
#include <filesystem>
#include <print>
#include <utils.hpp>

int main (void) {

    Logging::initialize(
        std::filesystem::current_path() / "examples/02-logging/logs"
    );

    LOG_DEBUG("Test log debugging");
    LOG_INFO("Test log info");
    LOG_ERROR("Test log error");
    LOG_WARN("Test log worn");

    Logging::shutdown();

    return 0;
}
