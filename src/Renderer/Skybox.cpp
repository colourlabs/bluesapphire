#include "Renderer/Skybox.h"
#include "Renderer/TextureUtils.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>

namespace BlueSapphire::Renderer {

float skyboxVertices[] = {
    // positions          
    // back face (-Z)
    -50.0f,  50.0f, -50.0f,
    -50.0f, -50.0f, -50.0f,
        50.0f, -50.0f, -50.0f,
        50.0f, -50.0f, -50.0f,
        50.0f,  50.0f, -50.0f,
    -50.0f,  50.0f, -50.0f,

    // front face (+Z)
    -50.0f, -50.0f,  50.0f,
    -50.0f,  50.0f,  50.0f,
        50.0f,  50.0f,  50.0f,
        50.0f,  50.0f,  50.0f,
        50.0f, -50.0f,  50.0f,
    -50.0f, -50.0f,  50.0f,

    // left face (-X)
    -50.0f,  50.0f,  50.0f,
    -50.0f,  50.0f, -50.0f,
    -50.0f, -50.0f, -50.0f,
    -50.0f, -50.0f, -50.0f,
    -50.0f, -50.0f,  50.0f,
    -50.0f,  50.0f,  50.0f,

    // right face (+X)
    50.0f,  50.0f, -50.0f,
    50.0f,  50.0f,  50.0f,
    50.0f, -50.0f,  50.0f,
    50.0f, -50.0f,  50.0f,
    50.0f, -50.0f, -50.0f,
    50.0f,  50.0f, -50.0f,

    // top face (+Y)
    -50.0f,  50.0f,  50.0f,
-50.0f,  50.0f, -50.0f,
    50.0f,  50.0f, -50.0f,
    50.0f,  50.0f, -50.0f,
    50.0f,  50.0f,  50.0f,
    -50.0f,  50.0f,  50.0f,

    // bottom face (-Y)
-50.0f, -50.0f, -50.0f,
-50.0f, -50.0f,  50.0f,
    50.0f, -50.0f,  50.0f,
    50.0f, -50.0f,  50.0f,
    50.0f, -50.0f, -50.0f,
    -50.0f, -50.0f, -50.0f
};

Skybox::Skybox(const std::vector<std::string>& faces) {
    cubemapTexture = LoadCubemap(faces);

    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    shader.LoadFromFile("shaders/skybox.vertex.glsl", "shaders/skybox.fragment.glsl");

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindVertexArray(0);
}

void Skybox::Draw(const glm::mat4& view, const glm::mat4& projection) {
    glDepthFunc(GL_LEQUAL);
    
    shader.Bind();
    shader.SetUniformMat4("view", glm::mat4(glm::mat3(view)));
    shader.SetUniformMat4("projection", projection);

    glBindVertexArray(vao);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);
}

}
