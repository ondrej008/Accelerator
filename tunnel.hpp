#pragma once

#include <vector>
#include <cmath>
#include <random>

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

struct Tunnel
{
Tunnel();

void update(double elapsed, glm::vec3 playerPos);
void render(GLint modelL, GLint colorL);

void generateNewSegment();
void generateNewObstacle(glm::vec3 pos);

bool playerCollide(glm::vec3 pos, float radius);

std::vector<TunnelSegment> m_segments;
std::vector<Obstacle*> m_obstacles;

glm::vec3 m_lastObstacle;
std::default_random_engine m_engine;
};