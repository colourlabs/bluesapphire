#pragma once

#include <vector>
#include <string>
#include <glad/glad.h>

namespace BlueSapphire::Renderer {

GLuint LoadCubemap(const std::vector<std::string>& faces);

}
