#pragma once

#include "obstacles.hpp"

// This class is an obstacle that does not move.
struct StaticObstacle : Obstacle
{
StaticObstacle(glm::vec3 pos, bool normal);
~StaticObstacle();

// Returns true if this obstacle collides with the player.
bool playerCollide(glm::vec3 pos, float radius);

// Returns true if this obstacle should be deleted, based on the player's position.
bool update(double elapsed, glm::vec3 pos);

// Renders this obstacle.
void render(GLint modelL, GLint colorL);

// Cuboids which handle the collision and contain the vertices for the obstacle.
Cuboid m_cuboid1;
Cuboid m_cuboid2;

// 4x4 matrices which contain the position of the cuboids.
glm::mat4 m_model1;
glm::mat4 m_model2;

// Vertex Buffer Objects for rendering the obstacle.
GLuint m_VBO1;
GLuint m_VBO2;

// Vertex Array Objects for rendering the obstacle.
GLuint m_VAO1;
GLuint m_VAO2;
};