#pragma once

#include <chrono>
#include <windows.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "utils.hpp"
#include "player.hpp"
#include "shader.hpp"
#include "tunnel.hpp"

// This class is where a lot of the things happen.
struct Accelerator
{
public:
	// The constructor initializes the GLFW, GLEW libraries,
	// creates a window and an OpenGL context,
	// that we can use to render onto the window.
	Accelerator();
	~Accelerator();

	// This function contains the game loop,
	void run();

	// These are callbacks for getting various events that happen to the window.
	// onWindowClose gets called when the window is closing by something the user did.
	// onWindowFocus gets called when the window is either in, or out of focus, which happens when the user tabs out (Alt + Tab). Pauses the game if it becomes out of focus.
	// onWindowSize gets called when the window is resized.
	// onCursorPosition is called when the mouse is moved, this updates the player's camera.
	// onKey is called when a key is pressed, ESC quits the program, SPACE restarts the game, W simulates a player colliding.
	void onWindowClose();
	void onWindowFocus(bool focused);
	void onWindowSize(int width, int height);
	void onCursorPosition(double xPos, double yPos);
	void onKey(int key, int scanCode, int action, int mods);

private:
	// This function is ran by the game loop, with "elapsed" being the
	// amount of time in seconds between the last frame and this frame.
	// Updates the tunnel and player.
	void update(double elapsed);

	// This function is ran by the game loop,
	// it tells the tunnel and player to render themselves.
	void render();

	// Pointers to the tunnel and player objects.
	Tunnel* m_tunnel;
	Player* m_player;

	// Stores the shader.
	Shader m_shader;

	// 4x4 matrix which is used to make the 3D effect of the game,
	// this is a perspective projection, 
	// which means that things get smaller the farther they are away.
	// m_fov is the field of view in degrees.
	glm::mat4 m_projection;
	float m_fov = 90.0;

	// Pointer to the window.
	GLFWwindow* m_window = nullptr;
	int m_width = 0;
	int m_height = 0;

	// Whether the game loop is running.
	bool m_running = true;

	// Whether the game is paused and we should stop rendering & updating.
	bool m_paused = false;

	// The number of frames that have elapsed.
	int m_frames = 0;

	// When did we start the game?
	std::chrono::high_resolution_clock::time_point m_start;

	// How long the game has been running for.
	double m_elapsed = 0;

	// This is for performance timing, these variables contain how much time was used to
	// render, update, swap the window and the total amount of time (which should be equal to m_elapsed).
	double m_render = 0;
	double m_update = 0;
	double m_swap = 0;
	double m_total = 0;

	// If vertical synchronization is enabled.
	bool m_vsync = true;
};