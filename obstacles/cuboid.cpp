#include "cuboid.hpp"

Cuboid::Cuboid(float width, float height, float length)
 : m_width(width), m_height(height), m_length(length)
{
	// add vertices to form a cuboid
	width /= 2.0;
	height /= 2.0;
	length /= 2.0;

	// front
	addVertex(-width, -height, length);
	addVertex(width, height, length);
	addVertex(-width, height, length);

	addVertex(-width, -height, length);
	addVertex(width, -height, length);
	addVertex(width, height, length);


	// back
	addVertex(-width, -height, -length);
	addVertex(-width, height, -length);
	addVertex(width, height, -length);
	
	addVertex(-width, -height, -length);
	addVertex(width, height, -length);
	addVertex(width, -height, -length);


	// left
	addVertex(-width, -height, -length);
	addVertex(-width, height, length);
	addVertex(-width, height, -length);

	addVertex(-width, -height, -length);
	addVertex(-width, -height, length);
	addVertex(-width, height, length);


	// right
	addVertex(width, -height, -length);
	addVertex(width, height, -length);
	addVertex(width, height, length);

	addVertex(width, -height, -length);
	addVertex(width, height, length);
	addVertex(width, -height, length);


	// top
	addVertex(-width, height, -length);
	addVertex(width, height, length);
	addVertex(width, height, -length);

	addVertex(-width, height, -length);
	addVertex(-width, height, length);
	addVertex(width, height, length);


	// bottom
	addVertex(-width, -height, -length);
	addVertex(width, -height, -length);
	addVertex(width, -height, length);

	addVertex(-width, -height, -length);
	addVertex(width, -height, length);
	addVertex(-width, -height, length);
}

void Cuboid::addVertex(float x, float y, float z)
{
	m_vertices.push_back(x);
	m_vertices.push_back(y);
	m_vertices.push_back(z);
}

bool Cuboid::collides(Cuboid other, glm::vec3 offset)
{
	float minX = -(m_width / 2.0);
	float maxX = (m_width / 2.0);

	float minY = -(m_height / 2.0);
	float maxY = (m_height / 2.0);

	float minZ = -(m_length / 2.0);
	float maxZ = (m_length / 2.0);

	float oMinX = -(other.m_width / 2.0) + offset.x;
	float oMaxX = (other.m_width / 2.0) + offset.x;

	float oMinY = -(other.m_height / 2.0) + offset.y;
	float oMaxY = (other.m_height / 2.0) + offset.y;

	float oMinZ = -(other.m_length / 2.0) + offset.z;
	float oMaxZ = (other.m_length / 2.0) + offset.z;

	return 
		(minX <= oMaxX && maxX >= oMinX) &&
		(minY <= oMaxY && maxY >= oMinY) &&
		(minZ <= oMaxZ && maxZ >= oMinZ);
}