#pragma once

#include <vector>
#include <string>
#include <glad/glad.h>

#include "Shader.h"

namespace BlueSapphire::Renderer {

class Skybox {
public:
    Skybox(const std::vector<std::string>& faces);
    void Draw(const glm::mat4& view, const glm::mat4& projection);

private:
    GLuint vao = 0, vbo = 0;
    GLuint cubemapTexture = 0;
    Shader shader;
};

}
