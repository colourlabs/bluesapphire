#pragma once

#include "GameObject.h"
#include "Physics/Collider.h"

#include <glm/glm.hpp>

namespace BlueSapphire::Physics {

class BoxCollider : public Collider {
public:
    glm::vec3 halfSize;

    BoxCollider(const glm::vec3& size) : halfSize(size / 2.0f) {}

    bool Intersects(const Collider& other, const GameObject* ownerA, const GameObject* ownerB) {
        const BoxCollider* otherBox = dynamic_cast<const BoxCollider*>(&other);
        if (!otherBox) return false;
    
        glm::vec3 aMin = GetMin(ownerA);
        glm::vec3 aMax = GetMax(ownerA);

        glm::vec3 bMin = otherBox->GetMin(ownerB);
        glm::vec3 bMax = otherBox->GetMax(ownerB);
    
        // what in the hell
        return (aMin.x <= bMax.x && aMax.x >= bMin.x) &&
               (aMin.y <= bMax.y && aMax.y >= bMin.y) &&
               (aMin.z <= bMax.z && aMax.z >= bMin.z);
    }        
};

};