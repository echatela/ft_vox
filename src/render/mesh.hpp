#pragma once

#include "data/vertex.hpp"

#include <vector>

class Mesh
{
	unsigned int _vao = 0;
	unsigned int _vbo = 0;
	unsigned int _ebo = 0;

	std::vector<Vertex>       _vertices;
	std::vector<unsigned int> _indices;

public:
	Mesh(const std::vector<Vertex>&       vertices,
	     const std::vector<unsigned int>& indices);
	~Mesh();

	void draw();

private:
	void setupMesh();
};
