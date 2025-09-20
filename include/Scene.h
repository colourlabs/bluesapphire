#pragma once

#include "GameObject.h"
#include "MeshRenderer.h"

#include <memory>
#include <vector>

namespace BlueSapphire {

class Scene {
public:
    std::vector<std::shared_ptr<GameObject>> objects;

    std::shared_ptr<GameObject> CreateObject(const std::string& name) {
        auto obj = std::make_shared<GameObject>(name);
        objects.push_back(obj);
        return obj;
    }


    void Update(float dt) {
        for (auto& obj : objects)
            obj->Update(dt);
    }

    void Render(const glm::mat4& view, const glm::mat4& projection) {
        for (auto& obj : objects) {
            for (auto& comp : obj->components) {
                auto renderer = std::dynamic_pointer_cast<MeshRenderer>(comp);
                if (renderer)
                    renderer->Draw(obj.get(), view, projection);
            }
        }
    }
};
        
};