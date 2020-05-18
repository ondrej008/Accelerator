#pragma once

#include <chrono>
#include <windows.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "utils.hpp"
#include "player.hpp"
#include "shader.hpp"

struct Accelerator
{
Accelerator();
~Accelerator();
void run();
void update(double elapsed);
void render();

void renderCrosshair();

void onWindowClose();
void onWindowFocus(bool focused);
void onCursorPosition(double xPos, double yPos);
void onKey(int key, int scanCode ,int action, int mods);

Player* m_player;

Shader m_shader;
glm::mat4 m_projection;

GLFWwindow* m_window = nullptr;
int m_width = 0;
int m_height = 0;

bool m_running = true;
bool m_paused = false;

int m_frames = 0;
std::chrono::high_resolution_clock::time_point m_start;
double m_elapsed = 0;
};