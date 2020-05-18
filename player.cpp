#include "player.hpp"

Player::Player()
{
    m_velocity = 1.0;
    m_radius = 0.05; // create a cube that's 0.1x0.1x0.1

    m_view = glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.0, 0.0));
    m_pos = glm::vec3(0.0, 0.0, 0.0);
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

    glGenBuffers(1, &VBO);

    glGenBuffers(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*) 0);
    glEnableVertexAttribArray(0);
}

void Player::addVertex(float x, float y, float z)
{
    vertices.push_back(x);
    vertices.push_back(y);
    vertices.push_back(z);
}

void Player::update(double elapsed)
{
    m_pos += (float) (m_velocity * elapsed) * m_cameraFront;

    m_view = glm::lookAt(m_pos, m_pos + m_cameraFront, m_cameraUp);
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

void Player::render(GLint modelL)
{
    if(m_velocity == 0.0)
    {
        glm::mat4 model = glm::translate(glm::mat4(1.0), (m_pos + (m_cameraFront * 2.0f)));
        glUniformMatrix4fv(modelL, 1, GL_FALSE, glm::value_ptr(model));

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    }
}

void Player::gameOver()
{
    if(m_velocity == 0.0)
        return;
    m_velocity = 0.0;
    m_pos += -2.0f * m_cameraFront;
}