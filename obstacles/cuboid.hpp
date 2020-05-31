#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include <vector>

// This class contains the vertices and dimensions of a cuboid, which is basically a 3D rectangle.
struct Cuboid
{
public:
	Cuboid(float width, float height, float length);
	~Cuboid();

	

	// This function returns true if this cuboid collides with the cuboid "other", 
	// offset is the difference in position between this cuboid and "other".
	bool collides(Cuboid other, glm::vec3 offset);

	float m_width;
	float m_height;
	float m_length;

	// List of vertices for this cuboid.
	std::vector<float> m_vertices;

private:
	// This function adds a vertex to the list of vertices.
	void addVertex(float x, float y, float z);
};