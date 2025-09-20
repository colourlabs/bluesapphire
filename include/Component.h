#pragma once

#include <glm/glm.hpp>

namespace BlueSapphire {

class Component {
public:
    virtual ~Component() = default;
    virtual void Update(float dt, class GameObject* owner) {}
    virtual void Draw(const class GameObject* owner, const glm::mat4& view, const glm::mat4& projection) {}
};
        
}