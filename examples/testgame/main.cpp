#include "App.h"
#include "AppSettings.h"
#include "Renderer/CameraModule.h"
#include "ModuleManager.h"
#include "Renderer/Skybox.h"
#include "Utilities/Logger.h"

#include <iostream>

class TestGame : public BlueSapphire::App {
public:
    std::shared_ptr<BlueSapphire::Renderer::CameraModule> cameraModule;
    std::unique_ptr<BlueSapphire::Renderer::Skybox> skybox;

    std::vector<std::string> skyboxFaces = {
        "assets/skybox/plainsky_right.jpg",
        "assets/skybox/plainsky_left.jpg",
        "assets/skybox/plainsky_up.jpg",
        "assets/skybox/plainsky_down.jpg",
        "assets/skybox/plainsky_front.jpg",
        "assets/skybox/plainsky_back.jpg"
    };
    
    void OnUpdate(float dt) override {
        float speed = 5.0f; // ups

        glm::vec3 forward = cameraModule->camera.GetFront();
        glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0, 1, 0)));
        glm::vec3 up = glm::vec3(0, 1, 0);

        BlueSapphire::Utils::Logger::Get().info("Camera pos: ({}, {}, {})", 
            cameraModule->camera.GetPosition().x,
            cameraModule->camera.GetPosition().y,
            cameraModule->camera.GetPosition().z
        );
    
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            cameraModule->camera.Move(forward * speed * dt);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            cameraModule->camera.Move(-forward * speed * dt);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            cameraModule->camera.Move(-right * speed * dt);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            cameraModule->camera.Move(right * speed * dt);
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            cameraModule->camera.Move(up * speed * dt);
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            cameraModule->camera.Move(-up * speed * dt);
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

        if (!cameraModule) {
            std::cerr << "CameraModule not found!" << std::endl;
            return false;
        }

        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        if (glfwRawMouseMotionSupported())
            glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

        glfwSetWindowUserPointer(window, this);
        glfwSetCursorPosCallback(window, [](GLFWwindow* win, double xpos, double ypos) {
            auto game = reinterpret_cast<TestGame*>(glfwGetWindowUserPointer(win));
            static bool firstMouse = true;
            static float lastX, lastY;

            if (firstMouse) {
                lastX = xpos;
                lastY = ypos;
                firstMouse = false;
            }

            float xoffset = xpos - lastX;
            float yoffset = lastY - ypos; // reversed Y
            lastX = xpos;
            lastY = ypos;

            game->cameraModule->camera.ProcessMouseMovement(xoffset, yoffset);
        });

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