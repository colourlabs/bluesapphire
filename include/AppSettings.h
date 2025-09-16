#pragma once

#include <string>

namespace BlueSapphire {

struct AppSettings {
    int width = 1280;
    int height = 720;
    std::string title = "BlueSapphireGame";

    bool vsync = true;
};

} // namespace BlueSapphire