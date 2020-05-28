#pragma once

#include "obstacles.hpp"

struct StaticObstacle : Obstacle
{
StaticObstacle(glm::vec3 pos, bool normal);

bool playerCollide(glm::vec3 pos, float radius);
bool update(double elapsed, glm::vec3 pos);
void render(GLint modelL, GLint colorL);

Cuboid m_cuboid1;
Cuboid m_cuboid2;

glm::mat4 m_model1;
glm::mat4 m_model2;

GLuint m_VBO1;
GLuint m_VBO2;

GLuint m_VAO1;
GLuint m_VAO2;
};