#pragma once

#include <unordered_map>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace BlueSapphire {

enum class KeyState {
    None,
    Pressed,
    Released,
    Held
};

class InputManager {
public:
    static InputManager& Get() {
        static InputManager instance;
        return instance;
    }

    void Initialize(GLFWwindow* window);
    void Update();
    void OnMouseMove(double xpos, double ypos);
    void OnScroll(double xoffset, double yoffset);

    glm::vec2 GetMouseDelta() const { return mouseDelta; }
    glm::vec2 GetScrollDelta() const { return scrollDelta; }

    void RegisterKey(int key) { keyStates[key] = KeyState::None; }
    KeyState GetKeyState(int key) { return keyStates[key]; }
    KeyState GetMouseButtonState(int button) { return mouseButtonStates[button]; }
    bool IsKeyPressed(int key) { return GetKeyState(key) == KeyState::Pressed; }
    bool IsKeyHeld(int key) { return GetKeyState(key) == KeyState::Held; }

    void RegisterMouseButton(int button) { mouseButtonStates[button] = KeyState::None; }
    bool IsMousePressed(int button) { return GetMouseButtonState(button) == KeyState::Pressed; }
    bool IsMouseHeld(int button) { return GetMouseButtonState(button) == KeyState::Held; }

    void SetMousePos(const glm::vec2& pos) {
        currentMousePos = pos;
        lastMousePos = pos;
    }

    void ResetMouseDelta() {
        mouseDelta = glm::vec2{0.0f};
    }

private:
    GLFWwindow* window = nullptr;

    std::unordered_map<int, KeyState> keyStates;
    std::unordered_map<int, KeyState> mouseButtonStates;

    glm::vec2 currentMousePos{0.0f};
    glm::vec2 lastMousePos{0.0f};
    glm::vec2 mouseDelta{0.0f};
    glm::vec2 scrollDelta{0.0f};
    glm::vec2 scrollOffset{0.0f};

    bool firstMouse = true;
};

};