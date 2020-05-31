#include "tunnel.hpp"

#include <cmath>
#include <random>
#include <chrono>
#include <memory>
#include <sstream>

Tunnel::Tunnel()
{
    unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
    m_engine = std::default_random_engine(seed);
    printToFile("Seeding with " + std::to_string(seed) + "\n");

    for(int i = 0; i < 15; i++)
    {
        generateNewSegment();
    }
}

Tunnel::~Tunnel()
{
    for(int i = 0; i < m_obstacles.size(); i++)
    {
        m_obstacles.erase(m_obstacles.begin() + i);
        i--;
    }
}

void Tunnel::generateNewSegment()
{
    if(m_segments.size() == 0)
    {
        TunnelSegment segment(10.0, glm::vec3(0.0, 0.0, -5.0));
        m_segments.push_back(segment);
        return;
    }

    glm::vec3 scale;
    glm::qua<float> orientation;
    glm::vec3 translation;
    glm::vec3 skew;
    glm::vec4 perspective;
    glm::decompose(m_segments.back().m_model, scale, orientation, translation, skew, perspective);

    TunnelSegment segment(10.0, glm::vec3(translation.x, translation.y, translation.z - m_segments.back().m_cylinder.getHeight() / 2.0 - 5.0));

    m_segments.push_back(segment);

    generateNewObstacle(translation);
}

void Tunnel::generateNewObstacle(glm::vec3 pos)
{
    // 20.0% chance for nothing
    // 40.0% chance for static obstacle
    // 0.0% chance for rotating obstacle
    // 40.0% chance for moving obstacle

    std::uniform_int_distribution<int> distribution(1, 10);

    int roll = distribution(m_engine);

    printToFile("Attempting to generate at: " + vec3ToString(pos) + ", rolled: " + std::to_string(roll) + "\n");

    switch(roll)
    {
        case 1:
        case 2:
            // nothing
            break;
        case 3:
        case 4:
        case 5:
        case 6:
        {
            roll = distribution(m_engine);
            StaticObstacle* so = new StaticObstacle(pos, roll > 5);
            m_obstacles.push_back(so);
            break;
        }
        case 7:
        case 8:
        case 9:
        case 10:
        {
            std::uniform_real_distribution<float> rng(0.1, 0.5);
            float speed = rng(m_engine);
            float vertical = rng(m_engine);
            MovingObstacle* mo = new MovingObstacle(pos, speed, vertical > 0.3);
            m_obstacles.push_back(mo);
            break;
        }
    }
}

void Tunnel::update(double elapsed, glm::vec3 playerPos)
{
    for(int i = 0; i < m_segments.size(); i++)
    {
        glm::vec3 scale;
        glm::qua<float> orientation;
        glm::vec3 translation;
        glm::vec3 skew;
        glm::vec4 perspective;
        glm::decompose(m_segments[i].m_model, scale, orientation, translation, skew, perspective);

        if(translation.z - playerPos.z - (m_segments[i].m_cylinder.getHeight()/2.0) > 0.5)
        {
            generateNewSegment();
            m_segments.erase(m_segments.begin() + i);
            i--;
        }
    }

    for(int i = 0; i < m_obstacles.size(); i++)
    {
        if(m_obstacles[i]->update(elapsed, playerPos))
        {
            m_obstacles.erase(m_obstacles.begin() + i);
            i--;
        }
    }
}

void Tunnel::render(GLint modelL, GLint colorL)
{
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glCullFace(GL_FRONT);

    glm::vec4 color = glm::vec4(0.4, 0.4, 0.4, 1.0);
    glUniform4fv(colorL, 1, glm::value_ptr(color));

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    for(int i = 0; i < m_segments.size(); i++)
    {
        m_segments[i].render(modelL);
    }

    color = glm::vec4(0.0, 0.0, 0.25, 1.0);
    glUniform4fv(colorL, 1, glm::value_ptr(color));

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    for(int i = 0; i < m_segments.size(); i++)
    {
        m_segments[i].render(modelL);
    }

    glCullFace(GL_BACK);

    for(int i = 0; i < m_obstacles.size(); i++)
    {
        m_obstacles[i]->render(modelL, colorL);
    }
}

bool Tunnel::playerCollide(glm::vec3 pos, float radius)
{
    // ignore Z
    glm::vec2 player(pos.x, pos.y);
    glm::vec2 tunnel(0.0, 0.0);
    if(glm::distance(player, tunnel) > (1.0 - radius))
    {
        return true;
    }

    for(int i = 0; i < m_obstacles.size(); i++)
    {
        if(m_obstacles[i]->playerCollide(pos, radius))
        {
            return true;
        }
    }

    return false;
}