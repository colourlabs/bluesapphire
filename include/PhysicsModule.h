#pragma once

#include "IModule.h"

#include "Physics/Collider.h"
#include "Physics/RigidBody.h"

#include <vector>

namespace BlueSapphire {

struct PhysicsEntry {
    Physics::RigidBody* rb;
    GameObject* owner;
};

// i hate myself. What in the fuck
class PhysicsModule : public IModule {
public:
    void Startup() override {}
    
    void Shutdown() override {
        rigidbodies.clear();
        colliders.clear();
    }

    void Update(float dt) {
        // integrate rigidbodies
        for (auto& entry : rigidbodies) {
            entry.rb->Integrate(dt);
        }

        // check collisions
        for (size_t i = 0; i < colliders.size(); i++) {
            for (size_t j = i + 1; j < colliders.size(); j++) {
                if (colliders[i]->Intersects(*colliders[j], colliders[i]->owner, colliders[j]->owner)) {
                    ResolveCollision(colliders[i], colliders[j]);
                }
            }
        }
    }

    void RegisterRigidBody(Physics::RigidBody* rb) {
        rigidbodies.push_back({rb, rb->owner});
    }
    
    void RegisterCollider(Physics::Collider* c) {
        colliders.push_back(c);
    }

    std::string GetName() const override { return "PhysicsModule"; }

private:
    std::vector<PhysicsEntry> rigidbodies;
    std::vector<Physics::Collider*> colliders;

    void ResolveCollision(Physics::Collider* a, Physics::Collider* b) {
        if (auto aRb = dynamic_cast<Physics::RigidBody*>(a)) aRb->velocity.y = 0.0f;
        if (auto bRb = dynamic_cast<Physics::RigidBody*>(b)) bRb->velocity.y = 0.0f;

        float overlapY = (a->GetMax(a->owner).y - b->GetMin(a->owner).y);

        if (overlapY > 0.0f) {
            a->owner->position.y -= overlapY * 0.5f;
            b->owner->position.y += overlapY * 0.5f;
        }
    }
};

}