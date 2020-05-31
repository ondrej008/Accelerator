#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include "../utils.hpp"

// This is an abstract class, which would be like an Interface in Java.
struct Obstacle
{
public:
	// This function should return true if the obstacle collides with the player.
	virtual bool playerCollide(glm::vec3 pos, float radius) = 0;

	// This function should return true if the obstacle should be deleted, 
	// it also handles any updates that the obstacle should need.
	virtual bool update(double elapsed, glm::vec3 playerPos) = 0;

	// This function should render the obstacle.
	virtual void render(GLint modelL, GLint colorL) = 0;

	// This is here so that memory does not leak.
	virtual ~Obstacle() {}
};