#pragma once

#include "AppSettings.h"
#include "Renderer/Shader.h"

#include <GLFW/glfw3.h>
#include <chrono>

namespace BlueSapphire {

class App {
public:
    virtual bool Initialize(const AppSettings& settings) = 0;
    void Run();
    void Shutdown();

    virtual void OnUpdate(float deltaTime) {}
    virtual void OnRender() {}
    virtual void OnFixedUpdate(float dt) {}
private:
    bool isRunning = false;

    using Clock = std::chrono::high_resolution_clock;
    Clock::time_point lastFrameTime;
protected:
    Renderer::Shader basicShader;
    GLFWwindow *window = nullptr;
};

} // namespace BlueSapphire