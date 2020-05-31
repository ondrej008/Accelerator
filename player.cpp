#include "player.hpp"

#include <iomanip>
#include <fstream>
#include <chrono>

Player::Player()
{
    m_velocity = 1.0;
    m_radius = 0.05; // create a cube that's 0.1x0.1x0.1

    m_view = glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.0, 0.0));
    m_pos = glm::vec3(0.0, 0.0, 0.0);
    m_cameraPos = glm::vec3(0.0, 0.0, 0.0);
    m_cameraFront = glm::vec3(0.0, 0.0, -1.0);
    m_cameraUp = glm::vec3(0.0, 1.0, 0.0);

    m_yaw = -90.0;
    m_pitch = 0.0;

    m_sensitivity = 0.33;

    addVertex(m_radius, m_radius, m_radius);
    addVertex(-m_radius, -m_radius, m_radius);
    addVertex(m_radius, -m_radius, m_radius);

    addVertex(-m_radius, m_radius, m_radius);
    addVertex(-m_radius, -m_radius, m_radius);
    addVertex(m_radius, m_radius, m_radius);


    addVertex(m_radius, m_radius, -m_radius);
    addVertex(m_radius, -m_radius, -m_radius);
    addVertex(-m_radius, -m_radius, -m_radius);

    addVertex(-m_radius, m_radius, -m_radius);
    addVertex(m_radius, m_radius, -m_radius);
    addVertex(-m_radius, -m_radius, -m_radius);


    addVertex(-m_radius, -m_radius, m_radius);
    addVertex(-m_radius, -m_radius, -m_radius);
    addVertex(m_radius, -m_radius, m_radius);
    
    addVertex(m_radius, -m_radius, -m_radius);
    addVertex(m_radius, -m_radius, m_radius);
    addVertex(-m_radius, -m_radius, -m_radius);


    addVertex(-m_radius, m_radius, m_radius);
    addVertex(m_radius, m_radius, m_radius);
    addVertex(-m_radius, m_radius, -m_radius);
    
    addVertex(m_radius, m_radius, -m_radius);
    addVertex(-m_radius, m_radius, -m_radius);
    addVertex(m_radius, m_radius, m_radius);


    addVertex(m_radius, -m_radius, m_radius);
    addVertex(m_radius, -m_radius, -m_radius);
    addVertex(m_radius, m_radius, m_radius);

    addVertex(m_radius, m_radius, -m_radius);
    addVertex(m_radius, m_radius, m_radius);
    addVertex(m_radius, -m_radius, -m_radius);


    addVertex(-m_radius, -m_radius, m_radius);
    addVertex(-m_radius, m_radius, m_radius);
    addVertex(-m_radius, -m_radius, -m_radius);

    addVertex(-m_radius, m_radius, -m_radius);
    addVertex(-m_radius, -m_radius, -m_radius);
    addVertex(-m_radius, m_radius, m_radius);

    glGenBuffers(1, &m_VBO);
    glGenVertexArrays(1, &m_VAO);

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

Player::~Player()
{
    glDeleteBuffers(1, &m_VBO);
    glDeleteVertexArrays(1, &m_VAO);
}

void Player::addVertex(float x, float y, float z)
{
    vertices.push_back(x);
    vertices.push_back(y);
    vertices.push_back(z);
}

void Player::update(double elapsed)
{
    if(m_velocity != 0.0)
    {
        m_pos += (float) (m_velocity * elapsed) * m_cameraFront;
        m_cameraPos += (float) (m_velocity * elapsed) * m_cameraFront;

        m_view = glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);

        

        if(m_velocity > 20.0)
        {
            std::ofstream file("highscore.txt", std::ios_base::app | std::ios_base::out);

            auto t = std::time(nullptr);
            auto tm = *std::localtime(&t);

            file << "[" << std::put_time(&tm, "%d-%m %H:%M:%S") << "]: " << "Reached maximum velocity at distance: " << std::abs(m_pos.z) << std::endl;

            file.close();

            m_velocity = 20.0;
        }
        else
        {
            m_velocity += 0.1 * elapsed;
        }
    }
}

void Player::updateCamera(double xOffset, double yOffset)
{
    //printToFile("Player::updateCamera(" + std::to_string(xOffset) + ", " + std::to_string(yOffset) + ")\n");
    
    m_yaw += xOffset * m_sensitivity;
    m_pitch += yOffset * m_sensitivity;

    if(m_pitch > 89.0)
        m_pitch = 89.0;
    if(m_pitch < -89.0)
        m_pitch = -89.0;
    
    const glm::vec3 worldFront = glm::vec3(0.0, 0.0, -1.0);
    const glm::vec3 worldUp = glm::vec3(0.0, 1.0, 0.0);

    glm::vec3 direction = glm::vec3(
        std::cos(glm::radians(m_yaw)) * std::cos(glm::radians(m_pitch)),
        std::sin(glm::radians(m_pitch)),
        std::sin(glm::radians(m_yaw)) * std::cos(glm::radians(m_pitch))
    );

    m_cameraFront = glm::normalize(direction);

    if(m_velocity != 0.0)
    {
        m_view = glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);
    }
    else
    {
        m_view = glm::lookAt(m_pos - m_cameraFront, m_pos, m_cameraUp);
    }
    
}

void Player::render(GLint modelL, GLint colorL)
{
    if(m_velocity == 0.0)
    {
        glm::mat4 model = glm::translate(glm::mat4(1.0), m_pos);
        glUniformMatrix4fv(modelL, 1, GL_FALSE, glm::value_ptr(model));
        
        glm::vec4 color(1.0, 1.0, 1.0, 0.5);
        glUniform4fv(colorL, 1, glm::value_ptr(color));

        glBindVertexArray(m_VAO);
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

void Player::gameOver()
{
    if(m_velocity == 0.0)
        return;

    std::ofstream file("highscore.txt", std::ios_base::app | std::ios_base::out);

    if(file.is_open())
    {
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);

        file << "[" << std::put_time(&tm, "%d-%m %H:%M:%S") << "]: " << "Distance: " << std::abs(m_pos.z) << ", velocity: " << m_velocity << std::endl;

        file.close();
    }

    m_velocity = 0.0;
    m_cameraPos += -0.5f * m_cameraFront;

    updateCamera(0.0, 0.0);
}