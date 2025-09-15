#pragma once

#include <chrono>
#include <string>

namespace BlueSapphire {

class App {
public:
    virtual bool Initialize(int width = 1280, int height = 720, const std::string& title = "BlueSapphireGame") = 0;
    void Run();
    void Shutdown();

    virtual void OnUpdate(float deltaTime) {}
    virtual void OnRender() {}
    virtual void OnFixedUpdate(float dt) {}
private:
    bool isRunning = false;

    using Clock = std::chrono::high_resolution_clock;
    Clock::time_point lastFrameTime;
};

} // namespace BlueSapphire