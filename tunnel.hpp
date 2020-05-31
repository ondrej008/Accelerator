#pragma once

#include <vector>
#include <cmath>
#include <random>
#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include "utils.hpp"
#include "segment.hpp"
#include "cylinder.hpp"
#include "obstacles/obstacles.hpp"

// This class represents the entirety of the tunnel,
// it contains the tunnel segments and obstacles.
struct Tunnel
{
Tunnel();
~Tunnel();

// This function updates the obstacles and deletes them if needed.
// It also deletes tunnel segments if they are no longer needed.
void update(double elapsed, glm::vec3 playerPos);

// This function renders the tunnel segments and obstacles.
void render(GLint modelL, GLint colorL);

// This function generates a new tunnel segment and calls generateNewObstacle.
void generateNewSegment();

// This function has generates a random obstacle,
// 20% chance of nothing,
// 40% chance of static obstacle,
// 40% chance of moving obstacle.
void generateNewObstacle(glm::vec3 pos);

// This function returns true if the player collides with the tunnel itself,
// or one of it's obstacles.
bool playerCollide(glm::vec3 pos, float radius);

// These lists contain the tunnel segments and obstacles.
std::vector<TunnelSegment> m_segments;
std::vector<Obstacle*> m_obstacles;

// For generating random numbers used in generateNewObstacle.
std::default_random_engine m_engine;
};