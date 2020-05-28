#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include <vector>

struct Cuboid
{
Cuboid(float width, float height, float length);

void addVertex(float x, float y, float z);
bool collides(Cuboid other, glm::vec3 offset);

float m_width;
float m_height;
float m_length;

std::vector<float> m_vertices;
};