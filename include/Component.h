#pragma once

#include <glm/glm.hpp>

namespace BlueSapphire {

// what in the hell
class GameObject;

class Component {
public:
    virtual ~Component() = default;
    GameObject* owner = nullptr;
    virtual void Update(float d) {}
    virtual void Draw(const glm::mat4& view, const glm::mat4& projection) {}
};
        
}