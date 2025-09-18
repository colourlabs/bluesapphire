#include "Renderer/Shader.h"

#include <fstream>
#include <iostream>
#include <sstream>

namespace BlueSapphire::Renderer {

bool Shader::LoadFromFile(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath) {
    if (!std::filesystem::exists(vertexPath) || !std::filesystem::exists(fragmentPath)) {
        std::cerr << "Vertex shader file doesn't exist: " << vertexPath << std::endl;
        return false;
    }

    std::ifstream vertexFile(vertexPath);
    std::ifstream fragmentFile(fragmentPath);

    if (!vertexFile || !fragmentFile) {
        std::cerr << "Failed to open shader files: " << vertexPath << ", " << fragmentPath << std::endl;
    }

    std::stringstream vertexStream, fragmentStream;
    vertexStream << vertexFile.rdbuf();
    fragmentStream << fragmentFile.rdbuf();

    std::string vertexCode = vertexStream.str();
    std::string fragmentCode = fragmentStream.str();

    GLuint vertexShader, fragmentShader;

    if (!CompileShader(vertexCode.c_str(), GL_VERTEX_SHADER, vertexShader)) return false;
    if (!CompileShader(fragmentCode.c_str(), GL_FRAGMENT_SHADER, fragmentShader)) return false;

    if (!LinkProgram(vertexShader, fragmentShader)) return false;

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return true;
}

bool Shader::CompileShader(const char* source, GLenum type, GLuint& shaderID) {
    shaderID = glCreateShader(type);
    glShaderSource(shaderID, 1, &source, nullptr);
    glCompileShader(shaderID);

    GLint success;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
 
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shaderID, 512, nullptr, infoLog);
        std::cerr << "Shader compilation error: " << infoLog << std::endl;
        return false;
    }
 
    return true;
}

bool Shader::LinkProgram(GLuint vertexShader, GLuint fragmentShader) {
    programID = glCreateProgram();
    glAttachShader(programID, vertexShader);
    glAttachShader(programID, fragmentShader);
    glLinkProgram(programID);

    GLint success;
    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(programID, 512, nullptr, infoLog);
        std::cerr << "Shader linking error:\n" << infoLog << "\n";
        return false;
    }

    return true;
}

GLint Shader::GetUniformLocation(const std::string& name) {
    if (uniformCache.find(name) != uniformCache.end())
        return uniformCache[name];

    GLint location = glGetUniformLocation(programID, name.c_str());
    uniformCache[name] = location;
    return location;
}

void Shader::SetUniformInt(const std::string& name, int value) {
    glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetUniformFloat(const std::string& name, float value) {
    glUniform1f(GetUniformLocation(name), value);
}

void Shader::SetUniformVec3(const std::string& name, const glm::vec3& value) {
    glUniform3fv(GetUniformLocation(name), 1, &value[0]);
}

void Shader::SetUniformMat4(const std::string& name, const glm::mat4& value) {
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &value[0][0]);
}

};