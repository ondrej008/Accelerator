#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <fstream>
#include <sstream>

#include "utils.hpp"

struct Shader
{
bool loadFromFiles(std::string vertex, std::string fragment);

GLuint m_programID;
};