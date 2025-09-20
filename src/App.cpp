#include <glad/glad.h>

#include "ScriptingModule.h"
#include "Utilities/Logger.h"

#include <GLFW/glfw3.h>

#include "App.h"
#include "AppSettings.h"
#include "ModuleManager.h"
#include "InputManager.h"
#include "Renderer/CameraModule.h"

namespace BlueSapphire {

bool App::Initialize(const AppSettings& settings) {
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

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        Utils::Logger::Get().error("Failed to initialize GLAD");
        return false;
    }

    if (settings.vsync) {
        glfwSwapInterval(1);
    } else {
        glfwSwapInterval(0);
    }

    InputManager::Get().Initialize(window);

    lastFrameTime = Clock::now();
    isRunning = true;

    if (!basicShader.LoadFromFile("shaders/basic.vertex.glsl", "shaders/basic.fragment.glsl")) {
        Utils::Logger::Get().error("Failed to load basic shader");
        return false;
    }

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    ModuleManager::Get().RegisterModule(std::make_unique<ScriptingModule>());
    ModuleManager::Get().RegisterModule(std::make_unique<Renderer::CameraModule>());

    // startup all modules
    ModuleManager::Get().StartupAll();

    Utils::Logger::Get().info("Game application initialized: {} ({}x{})", settings.title, settings.width, settings.height);
    
    return true;
}

void App::SetCursorMode(bool enabled) {
    if (enabled) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    } else {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        if (glfwRawMouseMotionSupported())
            glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }
}

void App::Run() {
    const double fixedDelta = 1.0 / 60.0; // fixed updates at 60hz
    double accumulator = 0.0;

    auto cameraModule = std::dynamic_pointer_cast<Renderer::CameraModule>(
        ModuleManager::Get().GetModule("CameraModule")
    );

    auto scriptingModule = std::dynamic_pointer_cast<ScriptingModule>(
        ModuleManager::Get().GetModule("ScriptingModule")
    );

    if (!cameraModule) {
        Utils::Logger::Get().error("CameraModule not found or wrong type.");
        return;
    }

    if (!scriptingModule) {
        Utils::Logger::Get().error("ScriptingModule not found or wrong type.");
        return;
    }

    scriptingModule->Bind("Engine", "Camera", &cameraModule->camera);

    while (isRunning && !glfwWindowShouldClose(window)) {
        glfwPollEvents();

        InputManager::Get().Update();

        auto currentFrameTime = Clock::now();
        std::chrono::duration<float> deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

        accumulator += deltaTime.count();
        while (accumulator >= fixedDelta) {
            OnFixedUpdate(static_cast<float>(fixedDelta));
            accumulator -= fixedDelta;
        }

        cameraModule->Update(deltaTime.count());

        // update
        OnUpdate(deltaTime.count());
        scriptingModule->Update(deltaTime.count());

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        basicShader.Bind();

        // render
        OnRender();
        scriptingModule->Render();

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