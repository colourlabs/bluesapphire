#include "InputManager.h"

namespace BlueSapphire {

void InputManager::Initialize(GLFWwindow* window) {
    this->window = window;

    glfwSetWindowUserPointer(window, this);

    glfwSetCursorPosCallback(window, [](GLFWwindow* w, double xpos, double ypos) {
        auto input = reinterpret_cast<InputManager*>(glfwGetWindowUserPointer(w));
        input->OnMouseMove(xpos, ypos);
    });

    glfwSetScrollCallback(window, [](GLFWwindow* w, double xoffset, double yoffset){
        auto input = reinterpret_cast<InputManager*>(glfwGetWindowUserPointer(w));
        input->scrollOffset += glm::vec2(xoffset, yoffset);
    });
};

void InputManager::Update() {
    // update key states for keys
    for (auto& [key, state] : keyStates) {
        if (glfwGetKey(window, key) == GLFW_PRESS) {
            state = (state == KeyState::Pressed) ? KeyState::Held : KeyState::Pressed;
        } else {
            state = (state == KeyState::Released || state == KeyState::None) ? KeyState::None : KeyState::Released;
        }
    }

    // update mouse buttons
    for (auto& [button, state] : mouseButtonStates) {
        if (glfwGetMouseButton(window, button) == GLFW_PRESS) {
            state = (state == KeyState::Pressed) ? KeyState::Held : KeyState::Pressed;
        } else {
            state = (state == KeyState::Released || state == KeyState::None) ? KeyState::None : KeyState::Released;
        }
    }

    scrollDelta = scrollOffset;
    scrollOffset = glm::vec2(0.0f);
}

void InputManager::OnMouseMove(double xpos, double ypos) {
    if (firstMouse) {
        lastMousePos = glm::vec2(xpos, ypos);
        firstMouse = false;
        return;
    }

    float xoffset = static_cast<float>(xpos - lastMousePos.x);
    float yoffset = static_cast<float>(lastMousePos.y - ypos); // inverted y

    mouseDelta.x += xoffset;
    mouseDelta.y += yoffset;

    lastMousePos = glm::vec2(xpos, ypos);
}

void InputManager::OnScroll(double xoffset, double yoffset) {
    scrollDelta += glm::vec2(xoffset, yoffset);
}

}