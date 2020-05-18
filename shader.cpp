#include "shader.hpp"

bool Shader::loadFromFiles(std::string vertex, std::string fragment)
{
    printToFile("Attempting to load " + vertex + " and " + fragment + "\n");

    std::ifstream file(vertex, std::ios_base::in);

    if(!file.is_open())
    {
        printToFile("\nERROR WHILE TRYING TO OPEN FILE " + vertex + "\n\n");
        return false;
    }

    std::stringstream stream;
    stream << file.rdbuf();
    std::string vertexSource = stream.str();

    printToFile("Read \"" + vertexSource + "\"\nfrom " + vertex + "\n");

    file.close();
    stream.clear();
    stream.str("");
    
    file.open(fragment, std::ios_base::in);

    if(!file.is_open())
    {
        printToFile("\nERROR WHILE TRYING TO OPEN FILE " + fragment + "\n\n");
        return false;
    }

    stream << file.rdbuf();
    std::string fragmentSource = stream.str();

    printToFile("Read \"" + fragmentSource + "\"\nfrom " + fragment + "\n");

    file.close();
    stream.clear();
    stream.str("");

    const char* shaderPointer = vertexSource.c_str();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &shaderPointer, nullptr);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        printToFile("\nERROR TRYING TO BUILD VERTEX SHADER: " + std::string(infoLog) + "\n\n");
        return false;
    }

    shaderPointer = fragmentSource.c_str();

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &shaderPointer, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        printToFile("\nERROR TRYING TO BUILD FRAGMENT SHADER: " + std::string(infoLog) + "\n\n");
        return false;
    }

    m_programID = glCreateProgram();
    glAttachShader(m_programID, vertexShader);
    glAttachShader(m_programID, fragmentShader);

    glLinkProgram(m_programID);

    glGetProgramiv(m_programID, GL_LINK_STATUS, &success);

    if(!success)
    {
        glGetProgramInfoLog(m_programID, 512, nullptr, infoLog);
        printToFile("\nERROR TRYING TO BUILD PROGRAM: " + std::string(infoLog) + "\n\n");
        return false;
    }

    glDetachShader(m_programID, vertexShader);
    glDetachShader(m_programID, fragmentShader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return true;
}