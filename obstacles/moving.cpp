#include "moving.hpp"

MovingObstacle::MovingObstacle(glm::vec3 pos, float speed, bool vertical)
 : m_cuboid1(2.0, 2.0, 0.2), m_cuboid2(2.0, 2.0, 0.2), m_speed(speed), m_vertical(vertical)
{
	glm::vec3 offset(1.25, 0.0, 0.0);

	if(m_vertical)
	{
		offset = glm::vec3(0.0, 1.25, 0.0);
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

MovingObstacle::~MovingObstacle()
{
	glDeleteBuffers(1, &m_VBO1);
	glDeleteBuffers(1, &m_VBO2);

	glDeleteVertexArrays(1, &m_VAO1);
	glDeleteVertexArrays(1, &m_VAO2);
}

bool MovingObstacle::playerCollide(glm::vec3 pos, float radius)
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

bool MovingObstacle::update(double elapsed, glm::vec3 pos)
{
	glm::vec3 scale;
    glm::qua<float> orientation;
    glm::vec3 translation1;
    glm::vec3 skew;
    glm::vec4 perspective;

	glm::decompose(m_model1, scale, orientation, translation1, skew, perspective);

	if(translation1.z - pos.z > 1.0)
	{
		return true;
	}

	glm::vec3 translation2;

	glm::decompose(m_model2, scale, orientation, translation2, skew, perspective);

	if(!m_vertical)
	{
		if(translation1.x > 2.0 || translation2.x < -2.0)
		{
			m_speed *= -1.0;
		}

		m_model1 = glm::translate(glm::mat4(1.0), glm::vec3(m_speed * elapsed, 0.0, 0.0)) * m_model1;
		m_model2 = glm::translate(glm::mat4(1.0), glm::vec3(m_speed * elapsed, 0.0, 0.0)) * m_model2;
	}
	else
	{
		if(translation1.y > 2.0 || translation2.y < -2.0)
		{
			m_speed *= -1.0;
		}

		m_model1 = glm::translate(glm::mat4(1.0), glm::vec3(0.0, m_speed * elapsed, 0.0)) * m_model1;
		m_model2 = glm::translate(glm::mat4(1.0), glm::vec3(0.0, m_speed * elapsed, 0.0)) * m_model2;
	}

	return false;
}

void MovingObstacle::render(GLint modelL, GLint colorL)
{
	glUniformMatrix4fv(modelL, 1, GL_FALSE, glm::value_ptr(m_model1));
	glUniform4fv(colorL, 1, glm::value_ptr(glm::vec4(0.7, 0.0, 0.0, 1.0)));

	glBindVertexArray(m_VAO1);
	glDrawArrays(GL_TRIANGLES, 0, m_cuboid1.m_vertices.size());


	glUniformMatrix4fv(modelL, 1, GL_FALSE, glm::value_ptr(m_model2));

	glBindVertexArray(m_VAO2);
	glDrawArrays(GL_TRIANGLES, 0, m_cuboid2.m_vertices.size());
}