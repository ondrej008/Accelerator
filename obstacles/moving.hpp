#pragma once

#include "obstacles.hpp"

// This class is an obstacle that moves horizontally or vertically.
struct MovingObstacle : Obstacle
{
MovingObstacle(glm::vec3 pos, float speed, bool vertical);
~MovingObstacle();

// This function returns true if this obstacle collides with the player.
bool playerCollide(glm::vec3 pos, float radius);

// This function returns true if it should be deleted, based on the player's position.
// It also moves the obstacle by m_speed and depends on m_vertical.
bool update(double elapsed, glm::vec3 pos);

// This function renders the obstacle.
void render(GLint modelL, GLint colorL);

// Cuboids which handle the collision and store the vertices for the obstacle.
Cuboid m_cuboid1;
Cuboid m_cuboid2;

// The 4x4 matrices that contain the position of the cuboids.
glm::mat4 m_model1;
glm::mat4 m_model2;

// Vertex Buffer Objects for rendering the obstacle.
GLuint m_VBO1;
GLuint m_VBO2;

// Vertex Array Objects for rendering the obstacle.
GLuint m_VAO1;
GLuint m_VAO2;

// The speed at which the obstacle moves.
float m_speed;

// Should the obstacle move vertically?
bool m_vertical;
};