#include "App.h"
#include "AppSettings.h"
#include "Renderer/CameraModule.h"
#include "ModuleManager.h"
#include "Renderer/Skybox.h"
#include "Scene.h"
#include "ScriptingModule.h"

#include <iostream>

class TestGame : public BlueSapphire::App {
public:
    std::shared_ptr<BlueSapphire::Renderer::CameraModule> cameraModule;
    std::shared_ptr<BlueSapphire::ScriptingModule> scriptingModule;
    std::unique_ptr<BlueSapphire::Scene> scene;

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
        if (scene) scene->Update(dt);
    }

    std::shared_ptr<BlueSapphire::Mesh> CreateCubeMesh() {
        std::vector<BlueSapphire::Vertex> vertices = {
            // positions             // normals           // tex coords
            {{-0.5f, -0.5f, -0.5f}, {0.0f,  0.0f, -1.0f}, {0.0f, 0.0f}},
            {{ 0.5f, -0.5f, -0.5f}, {0.0f,  0.0f, -1.0f}, {1.0f, 0.0f}},
            {{ 0.5f,  0.5f, -0.5f}, {0.0f,  0.0f, -1.0f}, {1.0f, 1.0f}},
            {{-0.5f,  0.5f, -0.5f}, {0.0f,  0.0f, -1.0f}, {0.0f, 1.0f}},
    
            {{-0.5f, -0.5f,  0.5f}, {0.0f,  0.0f,  1.0f}, {0.0f, 0.0f}},
            {{ 0.5f, -0.5f,  0.5f}, {0.0f,  0.0f,  1.0f}, {1.0f, 0.0f}},
            {{ 0.5f,  0.5f,  0.5f}, {0.0f,  0.0f,  1.0f}, {1.0f, 1.0f}},
            {{-0.5f,  0.5f,  0.5f}, {0.0f,  0.0f,  1.0f}, {0.0f, 1.0f}},
    
            {{-0.5f,  0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 0.0f}},
            {{-0.5f,  0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 1.0f}},
            {{-0.5f, -0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, {0.0f, 1.0f}},
            {{-0.5f, -0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}, {0.0f, 0.0f}},
    
            {{ 0.5f,  0.5f,  0.5f}, {1.0f,  0.0f,  0.0f}, {1.0f, 0.0f}},
            {{ 0.5f,  0.5f, -0.5f}, {1.0f,  0.0f,  0.0f}, {1.0f, 1.0f}},
            {{ 0.5f, -0.5f, -0.5f}, {1.0f,  0.0f,  0.0f}, {0.0f, 1.0f}},
            {{ 0.5f, -0.5f,  0.5f}, {1.0f,  0.0f,  0.0f}, {0.0f, 0.0f}},
    
            {{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f,  0.0f}, {0.0f, 1.0f}},
            {{ 0.5f, -0.5f, -0.5f}, {0.0f, -1.0f,  0.0f}, {1.0f, 1.0f}},
            {{ 0.5f, -0.5f,  0.5f}, {0.0f, -1.0f,  0.0f}, {1.0f, 0.0f}},
            {{-0.5f, -0.5f,  0.5f}, {0.0f, -1.0f,  0.0f}, {0.0f, 0.0f}},
    
            {{-0.5f,  0.5f, -0.5f}, {0.0f,  1.0f,  0.0f}, {0.0f, 1.0f}},
            {{ 0.5f,  0.5f, -0.5f}, {0.0f,  1.0f,  0.0f}, {1.0f, 1.0f}},
            {{ 0.5f,  0.5f,  0.5f}, {0.0f,  1.0f,  0.0f}, {1.0f, 0.0f}},
            {{-0.5f,  0.5f,  0.5f}, {0.0f,  1.0f,  0.0f}, {0.0f, 0.0f}}
        };
    
        std::vector<unsigned int> indices = {
            0, 1, 2, 2, 3, 0,        // back
            4, 5, 6, 6, 7, 4,        // front
            8, 9, 10, 10, 11, 8,     // left
            12, 13, 14, 14, 15, 12,  // right
            16, 17, 18, 18, 19, 16,  // bottom
            20, 21, 22, 22, 23, 20   // top
        };
    
        return std::make_shared<BlueSapphire::Mesh>(vertices, indices);
    }

    bool Initialize(const BlueSapphire::AppSettings& settings) override { 
        BlueSapphire::InputManager::Get().RegisterKey(GLFW_KEY_W);
        BlueSapphire::InputManager::Get().RegisterKey(GLFW_KEY_A);
        BlueSapphire::InputManager::Get().RegisterKey(GLFW_KEY_S);
        BlueSapphire::InputManager::Get().RegisterKey(GLFW_KEY_D);
        BlueSapphire::InputManager::Get().RegisterKey(GLFW_KEY_SPACE);
        BlueSapphire::InputManager::Get().RegisterKey(GLFW_KEY_LEFT_SHIFT);
        
        BlueSapphire::InputManager::Get().RegisterMouseButton(GLFW_MOUSE_BUTTON_LEFT);
        BlueSapphire::InputManager::Get().RegisterMouseButton(GLFW_MOUSE_BUTTON_RIGHT);
        
        if (!BlueSapphire::App::Initialize(settings))
            return false;

        SetCursorMode(false);

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
    
        scene = std::make_unique<BlueSapphire::Scene>();

        auto cubeMesh = CreateCubeMesh();
        auto cube = scene->CreateObject("Cube");
        cube->position = glm::vec3(0.0f, 0.0f, 0.0f);

        auto meshRenderer = cube->AddComponent<BlueSapphire::MeshRenderer>();
        meshRenderer->SetMesh(cubeMesh);
        meshRenderer->shader = std::shared_ptr<BlueSapphire::Renderer::Shader>(&basicShader, [](auto*){});

        return true;
    }
    
    void OnRender() override {
        if (scene) {
            scene->Render(
                cameraModule->camera.GetViewMatrix(),
                cameraModule->camera.GetProjectionMatrix()
            );
        }

        // draw skybox
        if (skybox)
            skybox->Draw(cameraModule->camera.GetViewMatrix(), cameraModule->camera.GetProjectionMatrix());
    }    

    void OnFixedUpdate(float dt) override {}
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