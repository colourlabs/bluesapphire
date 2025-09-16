#include "App.h"
#include "AppSettings.h"
#include "utils/Logger.h"
#include "ModuleManager.h"

#include <GLFW/glfw3.h>

namespace BlueSapphire {

bool App::Initialize(const AppSettings& settings) {
    // TODO: initalize opengl and other stuff etc etc
    Utils::Logger::Initialize();

    glfwSetErrorCallback([](int error, const char* desc) {
        Utils::Logger::Get().error("GLFW Error ({}): {}", error, desc);
    });    

    if (!glfwInit()) {
        Utils::Logger::Get().error("Failed to initialize GLFW");
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(settings.width, settings.height, settings.title.c_str(), NULL, NULL);
    if (!window) {
        Utils::Logger::Get().error("Failed to create GLFW window");
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);

    if (settings.vsync) {
        glfwSwapInterval(1);
    } else {
        glfwSwapInterval(0);
    }

    lastFrameTime = Clock::now();
    isRunning = true;

    // startup all modules
    ModuleManager::Get().StartupAll();

    Utils::Logger::Get().info("Game application initialized: {} ({}x{})", settings.title, settings.width, settings.height);
    
    return true;
}

void App::Run() {
    const double fixedDelta = 1.0 / 60.0; // fixed updates at 60hz
    double accumulator = 0.0;

    while (isRunning && !glfwWindowShouldClose(window)) {
        glfwPollEvents();

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

        glfwSwapBuffers(window);
    }
}

void App::Shutdown() {
    if (isRunning) { 
        isRunning = false;
        Utils::Logger::Get().info("Shutting down the game application.");

        glfwDestroyWindow(window);
        glfwTerminate();
        Utils::Logger::Get().info("GLFW terminated");
        
        spdlog::shutdown();
    };
}

}; // namespace BlueSapphire