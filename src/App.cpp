#include "App.h"
#include "utils/Logger.h"

namespace BlueSapphire {

bool App::Initialize(int width, int height, const std::string& title) {
    // TODO: initalize opengl and other stuff etc etc
    Utils::Logger::Initialize();

    lastFrameTime = Clock::now();
    isRunning = true;

    Utils::Logger::Get().info("Game application initialized: {} ({}x{})", title, width, height);
    
    return true;
}

void App::Run() {
    const double fixedDelta = 1.0 / 60.0; // fixed updates at 60hz
    double accumulator = 0.0;

    while (isRunning) {
        auto currentFrameTime = Clock::now();
        std::chrono::duration<float> deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

        accumulator += deltaTime.count();
        while (accumulator >= fixedDelta) {
            OnFixedUpdate(static_cast<float>(fixedDelta));
            accumulator -= fixedDelta;
        }

        // update
        OnUpdate(deltaTime.count());

        // render
        OnRender();

        // don't use 100% of the cpu Oh my lord i hate my pc fans
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void App::Shutdown() {
    if (isRunning) { 
        isRunning = false;
        Utils::Logger::Get().info("Shutting down the game application.");
        
        spdlog::shutdown();
    };
}

}; // namespace BlueSapphire