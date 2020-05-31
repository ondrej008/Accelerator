#pragma once

#include <windows.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cmath>
#include <vector>

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include "utils.hpp"

// This class contains the position information for the player,
// updates the player's position based on the player's velocity,
// renders the player's hitbox once it collides with something,
// manages the camera (view matrix) by receiving mouse input.
struct Player
{
public:
	Player();
	~Player();

	// This function updates the player's position based on the velocity,
	// and also increases the player's velocity until it reaches the maximum (20).
	void update(double elapsed);

	// This function rotates the camera.
	void updateCamera(double xOffset, double yOffset);

	// This function renders the player's hitbox, if it has collided with something.
	void render(GLint modelL, GLint colorL);

	// This function is called when the player has collided with something,
	// it sets the player's velocity to 0, so that it does not move.
	void gameOver();

	// Adds a vertex to the list of vertices, for the player's hitbox.
	void addVertex(float x, float y, float z);

	// These are for rendering the player's hitbox.
	GLuint m_VAO;
	GLuint m_VBO;

	// Contains the vertices of the player's hitbox.
	std::vector<float> vertices;

	// How fast the player is travelling.
	float m_velocity;

	// Size of the player's hitbox.
	float m_radius;

	// This is the position of the player.
	glm::vec3 m_pos;

	// This is the 4x4 matrix for the camera.
	glm::mat4 m_view;

	// These represent the camera's position, which way is the camera's front, and which way is up.
	glm::vec3 m_cameraPos;
	glm::vec3 m_cameraFront;
	glm::vec3 m_cameraUp;

	// The angles of the camera.
	double m_yaw;
	double m_pitch;

	// How much the mouse input changes the camera, currently 0.33
	double m_sensitivity;
};