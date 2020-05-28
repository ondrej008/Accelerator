#include "static.hpp"

StaticObstacle::StaticObstacle(glm::vec3 pos, bool normal)
 : m_cuboid1(1.0, 1.0, 0.2), m_cuboid2(1.0, 1.0, 0.2)
{
	glm::vec3 offset(0.5, 0.5, 0.0);

	if(!normal)
	{
		offset.x *= -1.0;
	}

	m_model1 = glm::translate(glm::mat4(1.0), pos + offset);
	m_model2 = glm::translate(glm::mat4(1.0), pos - offset);


	glGenBuffers(1, &m_VBO1);
	glGenVertexArrays(1, &m_VAO1);

	glBindVertexArray(m_VAO1);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO1);
	glBufferData(GL_ARRAY_BUFFER, m_cuboid1.m_vertices.size() * sizeof(float), m_cuboid1.m_vertices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);


	glGenBuffers(1, &m_VBO2);
	glGenVertexArrays(1, &m_VAO2);

	glBindVertexArray(m_VAO2);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO2);
	glBufferData(GL_ARRAY_BUFFER, m_cuboid2.m_vertices.size() * sizeof(float), m_cuboid2.m_vertices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

bool StaticObstacle::playerCollide(glm::vec3 pos, float radius)
{
	Cuboid player(radius, radius, radius);

	glm::vec3 scale;
    glm::qua<float> orientation;
    glm::vec3 translation;
    glm::vec3 skew;
    glm::vec4 perspective;

	glm::decompose(m_model1, scale, orientation, translation, skew, perspective);

	glm::vec3 playerOffset1 = translation - pos;

	glm::decompose(m_model2, scale, orientation, translation, skew, perspective);

	glm::vec3 playerOffset2 = translation - pos;

	return m_cuboid1.collides(player, playerOffset1) || m_cuboid2.collides(player, playerOffset2);
}

bool StaticObstacle::update(double elapsed, glm::vec3 pos)
{
	glm::vec3 scale;
    glm::qua<float> orientation;
    glm::vec3 translation;
    glm::vec3 skew;
    glm::vec4 perspective;

	glm::decompose(m_model1, scale, orientation, translation, skew, perspective);

	if(translation.z - pos.z > 1.0)
	{
		return true;
	}

	return false;
}

void StaticObstacle::render(GLint modelL, GLint colorL)
{
	glUniformMatrix4fv(modelL, 1, GL_FALSE, glm::value_ptr(m_model1));
	glUniform4fv(colorL, 1, glm::value_ptr(glm::vec4(0.7, 0.0, 0.0, 1.0)));

	glBindVertexArray(m_VAO1);
	glDrawArrays(GL_TRIANGLES, 0, m_cuboid1.m_vertices.size());


	glUniformMatrix4fv(modelL, 1, GL_FALSE, glm::value_ptr(m_model2));

	glBindVertexArray(m_VAO2);
	glDrawArrays(GL_TRIANGLES, 0, m_cuboid2.m_vertices.size());
}