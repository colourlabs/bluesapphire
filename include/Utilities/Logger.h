#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace BlueSapphire::Utils {

class Logger {
public:
    static void Initialize() {
        std::vector<spdlog::sink_ptr> sinks;

        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        console_sink->set_level(spdlog::level::info);
        sinks.push_back(console_sink);

        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/engine.log", true);
        file_sink->set_level(spdlog::level::info);
        sinks.push_back(file_sink);

        s_Logger = std::make_shared<spdlog::logger>("BlueSapphire", sinks.begin(), sinks.end());
        s_Logger->set_level(spdlog::level::info);
        spdlog::register_logger(s_Logger);
    }

    static spdlog::logger& Get() { return *s_Logger; }

private:
    static std::shared_ptr<spdlog::logger> s_Logger;
};

} // namespace BlueSapphire::Utils