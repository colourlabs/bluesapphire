#include "App.h"

namespace BlueSapphire {

bool App::Initialize(int width, int height, const std::string& title) {
    // TODO: initalize opengl and other stuff etc etc
  
    lastFrameTime = Clock::now();
    isRunning = true;

    return true;
}

void App::Run() {
    const double fixedDelta = 1.0 / 60.0; // fixed updates at 60hz
    double accumulator = 0.0;

    while (isRunning) {
        auto currentFrameTime = Clock::now();
        std::chrono::duration<float> deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

        while (accumulator >= fixedDelta) {
            OnFixedUpdate(static_cast<float>(fixedDelta));
            accumulator -= fixedDelta;
        }

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