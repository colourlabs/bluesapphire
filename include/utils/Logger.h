#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace BlueSapphire::Utils {

class Logger {
public:
    static void Initialize() {
        s_Logger = spdlog::basic_logger_mt("BlueSapphire", "logs/engine.log");
        s_Logger->set_level(spdlog::level::info);
    }

    static spdlog::logger& Get() { return *s_Logger; }

private:
    static std::shared_ptr<spdlog::logger> s_Logger;
};

} // namespace BlueSapphire::Utils