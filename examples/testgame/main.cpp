#include "App.h"
#include "AppSettings.h"
#include "Renderer/CameraModule.h"
#include "ModuleManager.h"
#include "Renderer/Skybox.h"
#include "ScriptingModule.h"

#include <iostream>

class TestGame : public BlueSapphire::App {
public:
    std::shared_ptr<BlueSapphire::Renderer::CameraModule> cameraModule;
    std::shared_ptr<BlueSapphire::ScriptingModule> scriptingModule;

    std::unique_ptr<BlueSapphire::Renderer::Skybox> skybox;

    std::vector<std::string> skyboxFaces = {
        "assets/skybox/clouds1_east.jpg",
        "assets/skybox/clouds1_west.jpg",  
        "assets/skybox/clouds1_up.jpg",    
        "assets/skybox/clouds1_down.jpg",   
        "assets/skybox/clouds1_north.jpg",
        "assets/skybox/clouds1_south.jpg" 
    };

    void OnUpdate(float dt) override {
        // this stuff runs every frame
    }

    bool Initialize(const BlueSapphire::AppSettings& settings) override {    
        if (!BlueSapphire::App::Initialize(settings))
            return false;

        lastX = settings.width / 2.0f;
        lastY = settings.height / 2.0f;

        skybox = std::make_unique<BlueSapphire::Renderer::Skybox>(skyboxFaces);

        cameraModule = std::dynamic_pointer_cast<BlueSapphire::Renderer::CameraModule>(
            BlueSapphire::ModuleManager::Get().GetModule("CameraModule")
        );

        scriptingModule = std::dynamic_pointer_cast<BlueSapphire::ScriptingModule>(
            BlueSapphire::ModuleManager::Get().GetModule("ScriptingModule")
        );

        if (!cameraModule) {
            std::cerr << "CameraModule not found!" << std::endl;
            return false;
        } 

        if (!scriptingModule) {
            std::cerr << "ScriptingModule not found!" << std::endl;
            return false;
        }

        // load lua scripts
        scriptingModule->LoadScript("scripts/init.lua");

        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        if (glfwRawMouseMotionSupported())
            glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

        return true;
    }
    
    void OnRender() override {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
        basicShader.Bind();

        // draw skybox
        if (skybox)
            skybox->Draw(cameraModule->camera.GetViewMatrix(), cameraModule->camera.GetProjectionMatrix());
    }    

    void OnFixedUpdate(float dt) override {}

private:
    float lastX;
    float lastY;
};
    

int main() {
    TestGame game;
    BlueSapphire::AppSettings settings;

    settings.width = 800;
    settings.height = 600;
    settings.title = "Test Game";
    settings.vsync = true;

    if (game.Initialize(settings)) {
        game.Run();
        game.Shutdown();
    } else {
        return -1;
    };

    return 0;
}