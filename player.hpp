#pragma once

#include <windows.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cmath>

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include "utils.hpp"

struct Player
{
Player();
void update(double elapsed);
void updateCamera(double xOffset, double yOffset);
void render();

float m_velocity;
float m_radius;

glm::mat4 m_view;
glm::vec3 m_pos;
glm::vec3 m_cameraFront;
glm::vec3 m_cameraUp;

double m_yaw;
double m_pitch;
double m_sensitivity;
};