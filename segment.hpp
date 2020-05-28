#pragma once

#include "cylinder.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

struct TunnelSegment
{
TunnelSegment(float height, glm::vec3 pos);

void update(double elapsed);
void render(GLint modelL);

glm::mat4 m_model;

Cylinder m_cyl;
};