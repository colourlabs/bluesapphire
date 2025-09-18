#pragma once

#include <filesystem>
#include <unordered_map>
#include <glm/glm.hpp>
#include <glad/glad.h>

namespace BlueSapphire::Renderer {

class Shader {
public:
    Shader() = default;
    ~Shader() = default;

    bool LoadFromFile(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath);

    void Bind() const { glUseProgram(programID); }
    void Unbind() const { glUseProgram(0); }

    void SetUniformInt(const std::string& name, int value);
    void SetUniformFloat(const std::string& name, float value);
    void SetUniformVec3(const std::string& name, const glm::vec3& value);
    void SetUniformMat4(const std::string& name, const glm::mat4& value);

private:
    GLuint programID = 0;
    std::unordered_map<std::string, GLint> uniformCache;

    GLint GetUniformLocation(const std::string& name);
    bool CompileShader(const char* source, GLenum type, GLuint& shaderID);
    bool LinkProgram(GLuint vertexShader, GLuint fragmentShader);
};

};