#pragma once

#include "cylinder.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

// This class represents a section of the tunnel.
struct TunnelSegment
{
TunnelSegment(float height, glm::vec3 pos);

// This does nothing currently.
void update(double elapsed);

// This function renders the cylinder's walls.
void render(GLint modelL);

// This 4x4 matrix contains the position of the segment.
glm::mat4 m_model;

// Cylinder that we render.
Cylinder m_cyl;
};