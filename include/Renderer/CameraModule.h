#pragma once

#include "IModule.h"
#include "Renderer/Camera.h"

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

    void Update(float deltaTime) {}

    Renderer::Camera camera;
};

}