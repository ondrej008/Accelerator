#pragma once

#include <chrono>
#include <windows.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "utils.hpp"
#include "player.hpp"

struct Accelerator
{
Accelerator();
~Accelerator();
void run();
void update(double elapsed);
void render();

void onWindowClose();
void onWindowFocus(bool focused);
void onCursorPosition(double xPos, double yPos);

Player m_player;

glm::mat4 projection;

GLFWwindow* m_window = nullptr;

bool m_running = true;
bool m_paused = false;

int m_frames = 0;
std::chrono::high_resolution_clock::time_point m_start;
double m_elapsed = 0;
};