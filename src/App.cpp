#include "App.h"

namespace BlueSapphire {

bool App::Initialize(int width, int height, const std::string& title) {
    // TODO: initalize opengl and other stuff etc etc
  
    lastFrameTime = Clock::now();
    isRunning = true;

    return true;
}

void App::Run() {
    const double fixedDelta = 1.0 / 60.0; // physics updates at 60hz

    while (isRunning) {
        auto currentFrameTime = Clock::now();
        std::chrono::duration<float> deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

        // update
        OnUpdate(deltaTime.count());

        // render
        OnRender();
    }
}

void App::Shutdown() {
    // TODO: start more shutdown stuff here, this is just a placeholder
    if (isRunning) isRunning = false;
}    

}; // namespace BlueSapphire