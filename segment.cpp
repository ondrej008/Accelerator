#include "segment.hpp"

TunnelSegment::TunnelSegment(float height, glm::vec3 pos)
 : m_cyl(1.0 ,1.0, height, 36, 4, true), m_model(glm::translate(glm::mat4(1.0), pos))
{
    
}

void TunnelSegment::render(GLint modelL)
{
    glUniformMatrix4fv(modelL, 1, GL_FALSE, glm::value_ptr(m_model));

    m_cyl.drawSide();
}