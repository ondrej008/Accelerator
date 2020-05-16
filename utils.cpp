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

void clearFile()
{
    std::ofstream file("out.txt", std::ios_base::out);
    file.close();
}