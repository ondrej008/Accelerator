#include "player.hpp"

Player::Player()
{
    m_velocity = 0.01;
    m_radius = 0.0;

    m_view = glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.0, 0.0));
    m_pos = glm::vec3(0.0, 0.0, 0.0);
    m_cameraFront = glm::vec3(0.0, 0.0, -1.0);
    m_cameraUp = glm::vec3(0.0, 1.0, 0.0);

    m_yaw = 0.0;
    m_pitch = 0.0;

    m_sensitivity = 1.0;
}

void Player::update(double elapsed)
{
    m_pos += (float) (m_velocity * elapsed) * m_cameraFront;
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

    m_view = glm::lookAt(m_pos, m_pos + m_cameraFront, m_cameraUp);
}

void Player::render()
{

}