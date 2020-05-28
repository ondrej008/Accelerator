#pragma once

#include <string>
#include <glm/glm.hpp>

#define START_TIMING() auto _start = std::chrono::high_resolution_clock::now()
#define RECORD_TIMING(x) auto _end = std::chrono::high_resolution_clock::now(); printToFile(x + std::string(": ") + std::to_string((_end - _start).count() / std::pow(10.0, 6.0)) + "\n")

void printToFile(std::string str);
std::string vec3ToString(glm::vec3 pos);
void clearFile();