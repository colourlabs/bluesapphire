#pragma once

#define SOL_USE_STD_OPTIONAL 1

#include "IModule.h"
#include "Utilities/Logger.h"
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
                if (data) result = func(data);
                else result = func();
                if (!result.valid()) {
                    sol::error err = result;
                    Utils::Logger::Get().error("[lua Event Error] {}", err.what());
                }
            }
        }
    }
};

} // namespace BlueSapphire
