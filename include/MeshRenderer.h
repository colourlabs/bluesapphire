#pragma once 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "GameObject.h"
#include "Mesh.h"
#include "Renderer/Shader.h"

namespace BlueSapphire {

class MeshRenderer : public Component {
public:
    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Renderer::Shader> shader;

    void Update(float dt, GameObject* owner) {}

    void Draw(const GameObject* owner, const glm::mat4& view, const glm::mat4& projection) {
        shader->Bind();

        glm::mat4 model = glm::translate(glm::mat4(1.0f), owner->position)
                        * glm::rotate(glm::mat4(1.0f), owner->rotation.y, glm::vec3(0.0f, 1.0f, 0.0f))
                        * glm::rotate(glm::mat4(1.0f), owner->rotation.x, glm::vec3(1.0f, 0.0f, 0.0f))
                        * glm::rotate(glm::mat4(1.0f), owner->rotation.z, glm::vec3(0.0f, 0.0f, 1.0f))
                        * glm::scale(glm::mat4(1.0f), owner->scale);

        shader->SetUniformMat4("model", model);
        shader->SetUniformMat4("view", view);
        shader->SetUniformMat4("projection", projection);

        mesh->Draw();
    }

    void SetMesh(std::shared_ptr<Mesh> m) {
        mesh = m;
    }
};
    

};