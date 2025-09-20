#pragma once

#include "IModule.h"
#include "Renderer/Camera.h"
#include "ModuleManager.h"
#include "InputManager.h"

#include <glm/glm.hpp>

namespace BlueSapphire::Renderer {

class CameraModule : public IModule {
public:
    CameraModule() = default;
    ~CameraModule() override = default;

    void Startup() override {
        camera.SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));
        camera.SetRotation(0.0f, -90.0f);
        camera.SetFOV(60.0f);            
        camera.SetAspectRatio(16.0f / 9.0f);
        camera.SetNearFarPlanes(0.1f, 100.0f);
    }

    void Shutdown() override {}

    std::string GetName() const override {
        return "CameraModule";
    }

    void Update(float deltaTime) {
        auto cameraModule = std::dynamic_pointer_cast<Renderer::CameraModule>(
            ModuleManager::Get().GetModule("CameraModule")
        );
        if (!cameraModule) return;
    
        float speed = 5.0f;
        glm::vec3 forward = cameraModule->camera.GetFront();
        glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0, 1, 0)));
        glm::vec3 up = glm::vec3(0, 1, 0);
    
        auto& input = InputManager::Get();
        if (input.IsKeyHeld(GLFW_KEY_W)) cameraModule->camera.Move(forward * speed * deltaTime);
        if (input.IsKeyHeld(GLFW_KEY_S)) cameraModule->camera.Move(-forward * speed * deltaTime);
        if (input.IsKeyHeld(GLFW_KEY_A)) cameraModule->camera.Move(-right * speed * deltaTime);
        if (input.IsKeyHeld(GLFW_KEY_D)) cameraModule->camera.Move(right * speed * deltaTime);
        if (input.IsKeyHeld(GLFW_KEY_SPACE)) cameraModule->camera.Move(up * speed * deltaTime);
        if (input.IsKeyHeld(GLFW_KEY_LEFT_SHIFT)) cameraModule->camera.Move(-up * speed * deltaTime);
    
        glm::vec2 delta = input.GetMouseDelta();
        cameraModule->camera.ProcessMouseMovement(delta.x, delta.y, true);
        input.ResetMouseDelta();
    }

    Renderer::Camera camera;
};

}