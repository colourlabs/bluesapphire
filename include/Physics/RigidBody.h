#pragma once

#include "Component.h"
#include "GameObject.h"

namespace BlueSapphire::Physics {

class RigidBody : public BlueSapphire::Component {
public:
    glm::vec3 velocity = glm::vec3(0.0f);
    glm::vec3 acceleration = glm::vec3(0.0f);
    float mass = 1.0f;
    bool useGravity = true;
    bool isStatic = false;
    glm::vec3 gravity{0.0f, -9.8f, 0.0f};

    void ApplyForce(const glm::vec3& force) {
        acceleration += force / mass;
    }

    void Integrate(float dt) {
        if (isStatic) return;
        if (useGravity) acceleration += gravity;

        velocity += acceleration * dt;
        owner->position += velocity * dt;

        acceleration = glm::vec3(0.0f);
    }

    void Update(float dt) override {
        Integrate(dt);
    }
};

};