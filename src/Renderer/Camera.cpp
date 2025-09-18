#include "Renderer/Camera.h"

namespace BlueSapphire::Renderer {

Camera::Camera(const glm::vec3& position, float fov, float aspect, float nearPlane, float farPlane)
    : position(position), fov(fov), aspect(aspect), nearPlane(nearPlane), farPlane(farPlane) {
    UpdateView();
    UpdateProjection();
}

void Camera::UpdateProjection() {
    projection = glm::perspective(glm::radians(fov), aspect, nearPlane, farPlane);
}

void Camera::UpdateView() {
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);

    glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    view = glm::lookAt(position, position + front, worldUp);
}

void Camera::SetFOV(float f) {
    fov = f;
    UpdateProjection();
}

void Camera::SetAspectRatio(float a) {
    aspect = a;
    UpdateProjection();
}

void Camera::SetNearFarPlanes(float nearP, float farP) {
    nearPlane = nearP;
    farPlane = farP;
    UpdateProjection();
}

void Camera::SetPosition(const glm::vec3& pos) {
    position = pos;
    UpdateView();
}

void Camera::SetRotation(float pitchAngle, float yawAngle) {
    pitch = pitchAngle;
    yaw = yawAngle;
    UpdateView();
}

void Camera::Move(const glm::vec3& delta) {
    position += delta;
    UpdateView();
}

const glm::mat4& Camera::GetViewMatrix() const { return view; }
const glm::mat4& Camera::GetProjectionMatrix() const { return projection; }

glm::vec3 Camera::GetFront() const {
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    return glm::normalize(front);
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch) {
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    if (constrainPitch) {
        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;
    }

    UpdateView();
}

} // namespace BlueSapphire::Renderer