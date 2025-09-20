#pragma once

#define SOL_USE_STD_OPTIONAL 1

#include "IModule.h"
#include "Utilities/Logger.h"
#include "InputManager.h"
#include "Renderer/Camera.h"

#include <sol2/sol.hpp>
#include <unordered_map>
#include <vector>

namespace BlueSapphire {

enum class EventType {
    FrameUpdate,
    FixedUpdate,
    Render,
    Custom
};

struct Event {
    EventType type;
    void* data = nullptr;
};

class ScriptingModule : public IModule {
public:
    ScriptingModule() = default;
    ~ScriptingModule() override = default;

    void Startup() override {
        lua.open_libraries(sol::lib::base,
                           sol::lib::package,
                           sol::lib::math,
                           sol::lib::table,
                           sol::lib::string,
                           sol::lib::os);

        lua.create_named_table("Engine");
        lua.create_named_table("Hooks");
        lua.create_named_table("Input");

        lua.new_usertype<Renderer::Camera>(
            "Camera",
            "GetPosition", &Renderer::Camera::GetPosition,
            "GetFront", &Renderer::Camera::GetFront,
            "SetPosition", &Renderer::Camera::SetPosition,
            "SetRotation", &Renderer::Camera::SetRotation,
            "Move", &Renderer::Camera::Move
        );

        lua.new_usertype<InputManager>("InputManager",
            "IsKeyHeld", &InputManager::IsKeyHeld,
            "IsMousePressed", &InputManager::IsMousePressed,
            "GetKeyState", [](InputManager& self, int key) {
                return static_cast<int>(self.GetKeyState(key));
            },
            "GetMouseDelta", [](InputManager& self) {
                glm::vec2 delta = self.GetMouseDelta();
                return std::make_tuple(delta.x, delta.y);
            },
            "ResetMouseDelta", &InputManager::ResetMouseDelta
        );        

        lua.new_usertype<glm::vec3>("vec3",
            "x", &glm::vec3::x,
            "y", &glm::vec3::y,
            "z", &glm::vec3::z
        );        
        
        auto keys = lua.create_named_table("Keys");
        
        keys["W"] = GLFW_KEY_W;
        keys["A"] = GLFW_KEY_A;
        keys["S"] = GLFW_KEY_S;
        keys["D"] = GLFW_KEY_D;
        keys["SPACE"] = GLFW_KEY_SPACE;
        keys["LEFT_SHIFT"] = GLFW_KEY_LEFT_SHIFT;
        keys["ESCAPE"] = GLFW_KEY_ESCAPE;
        
        keys[sol::meta_function::new_index] = [](sol::object, sol::object, sol::object) {
            throw std::runtime_error("Keys table is read-only");
        };

        auto buttons = lua.create_named_table("MouseButtons");
        
        buttons["LEFT"] = GLFW_MOUSE_BUTTON_LEFT;
        buttons["RIGHT"] = GLFW_MOUSE_BUTTON_RIGHT;
        buttons["MIDDLE"] = GLFW_MOUSE_BUTTON_MIDDLE;

        buttons[sol::meta_function::new_index] = [](sol::object, sol::object, sol::object) {
            throw std::runtime_error("MouseButtons table is read-only");
        };

        lua["Engine"]["InputManager"] = &InputManager::Get();

        lua["Hooks"]["SubscribeFrameUpdate"] = [this](sol::function func) {
            Subscribe(EventType::FrameUpdate, func);
        };

        lua["Hooks"]["SubscribeFixedUpdate"] = [this](sol::function func) {
            Subscribe(EventType::FixedUpdate, func);
        };
        
        lua["Hooks"]["SubscribeRender"] = [this](sol::function func) {
            Subscribe(EventType::Render, func);
        };

        Utils::Logger::Get().info("ScriptingModule initialized.");
    }

    void Shutdown() override {
        lua.collect_garbage();
        Utils::Logger::Get().info("ScriptingModule shutdown.");
    }

    std::string GetName() const override { return "ScriptingModule"; }

    void Update(float deltaTime) {
        Emit(EventType::FrameUpdate, &deltaTime);
    }

    void FixedUpdate(float deltaTime) {
        Emit(EventType::FixedUpdate, &deltaTime);
    }

    void Render() {
        Emit(EventType::Render);
    }

    void LoadScript(const std::string& path) {
        sol::load_result script = lua.load_file(path);
        if (!script.valid()) {
            sol::error err = script;
            Utils::Logger::Get().error("Failed to load script {}: {}", path, err.what());
            return;
        }

        sol::protected_function_result result = script();
        if (!result.valid()) {
            sol::error err = result;
            Utils::Logger::Get().error("Failed to execute script {}: {}", path, err.what());
        }
    }

    template<typename T>
    void Bind(const std::string& table, const std::string& name, T* object) {
        if (!lua[table].valid()) {
            lua.create_named_table(table);
        }

        lua[table][name] = object;
    }

private:
    sol::state lua;
    std::unordered_map<EventType, std::vector<sol::protected_function>> listeners;

    void Subscribe(EventType type, sol::protected_function func) {
        listeners[type].push_back(func);
    }

    void Emit(EventType type, void* data = nullptr) {
        auto it = listeners.find(type);
        if (it != listeners.end()) {
            for (auto& func : it->second) {
                sol::protected_function_result result;
                
                if (data) {
                    float* dtPtr = static_cast<float*>(data);
                    result = func(*dtPtr);
                } else {
                    result = func();
                }    

                if (!result.valid()) {
                    sol::error err = result;
                    Utils::Logger::Get().error("[lua Event Error] {}", err.what());
                }
            }
        }
    }
};

} // namespace BlueSapphire
