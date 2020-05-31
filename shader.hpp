#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <fstream>
#include <sstream>

#include "utils.hpp"

// This class loads a vertex and fragment shader from file,
// then compiles and attaches them to a program,
// which can then be used by OpenGL.
struct Shader
{
// Reads files at path "vertex" and "fragment", compiles them and attaches them to a program.
bool loadFromFiles(std::string vertex, std::string fragment);

// The program ID that we got from the shaders.
GLuint m_programID;
};