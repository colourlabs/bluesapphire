#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace BlueSapphire::Renderer {

class Camera {
public:
    Camera(const glm::vec3& position = glm::vec3(0.0f),
            float fov = 45.0f, float aspect = 16.0f/9.0f,
            float nearPlane = 0.1f, float farPlane = 100.0f);

    void SetPosition(const glm::vec3& pos);
    void SetRotation(float pitchAngle, float yawAngle);
    void Move(const glm::vec3& delta);

    void SetFOV(float f);
    void SetAspectRatio(float a);
    void SetNearFarPlanes(float nearP, float farP);

    void UpdateProjection();
    void UpdateView();

    const glm::mat4& GetViewMatrix() const;
    const glm::mat4& GetProjectionMatrix() const;    
    glm::vec3 GetFront() const;
    glm::vec3 GetPosition() const { return position; }

    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);

private:
    glm::vec3 position{0.0f};
    float pitch = 0.0f, yaw = -90.0f;
    float fov, aspect, nearPlane, farPlane;
    glm::mat4 view{1.0f};
    glm::mat4 projection{1.0f};

    float mouseSensitivity = 0.1f;
};

}; // namespace BlueSapphire::Renderer
