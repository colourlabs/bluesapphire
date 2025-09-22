#pragma once 

#include "Component.h"

#include <glm/glm.hpp>
#include <memory>
#include <vector>

namespace BlueSapphire {

class GameObject {
public:
    std::string name;

    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale = glm::vec3(1.0f);

    std::vector<std::shared_ptr<Component>> components;

    GameObject(const std::string& name) : name(name) {}

    void Update(float dt) {
        for (auto& comp : components)
            comp->Update(dt);
    }

    template<typename T, typename... Args>
    std::shared_ptr<T> AddComponent(Args&&... args) {
        static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");

        auto comp = std::make_shared<T>(std::forward<Args>(args)...);
        comp->owner = this;
        components.push_back(comp);
        return comp;
    }
};

};