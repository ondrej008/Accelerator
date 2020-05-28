#include "utils.hpp"

#include <fstream>

void printToFile(std::string str)
{
    std::ofstream file;
    file.open("out.txt", std::ios_base::out | std::ios_base::app);

    file << str;

    file.flush();

    file.close();
}

std::string vec3ToString(glm::vec3 pos)
{
    return "{" + std::to_string(pos.x) + ", " + std::to_string(pos.y) + ", " + std::to_string(pos.z) + "}";
}

void clearFile()
{
    std::ofstream file("out.txt", std::ios_base::out);
    file.close();
}