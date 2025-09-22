#pragma once 

#include "Component.h"
#include "GameObject.h"

namespace BlueSapphire::Physics {

class Collider : public BlueSapphire::Component {
public:
    glm::vec3 halfSize;

    virtual bool Intersects(const Collider& other, const GameObject* ownerA, const GameObject* ownerB) = 0;

    glm::vec3 GetMin(const GameObject* owner) const {
        return owner->position - halfSize;
    }
    
    glm::vec3 GetMax(const GameObject* owner) const {
        return owner->position + halfSize;
    }
};

};