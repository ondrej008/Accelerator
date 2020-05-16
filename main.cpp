#include <fstream>
#include <string>

#include "accelerator.hpp"

int main(int argc, char** argv)
{
    clearFile();
    printToFile("argc: " + std::to_string(argc) + ", argv[0]: " + argv[0] + "\n");
    
    Accelerator accelerator;

    accelerator.run();
}