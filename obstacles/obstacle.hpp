#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include "../utils.hpp"

struct Obstacle
{
virtual bool playerCollide(glm::vec3 pos, float radius) = 0;
virtual bool update(double elapsed, glm::vec3 playerPos) = 0;
virtual void render(GLint modelL, GLint colorL) = 0;

virtual ~Obstacle() {}
};