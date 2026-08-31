#pragma once

#include "render/shader.hpp"
#include <vector>
static constexpr int kChunkWidth = 16;
static constexpr int kChunkHeight = 256;
static constexpr int kChunkSize = kChunkWidth * kChunkHeight * kChunkWidth;

struct Vertex
{
	glm::vec3 pos;
	int       face;
	int       i;
};

class Chunk
{
	bool _voxels[kChunkSize]{true};

	unsigned int _vao;
	unsigned int _vbo;

	std::vector<Vertex> _vertices;

public:
	Chunk();

	void draw(Shader& shader);
	// bool at(int x, int y, int z) const;

private:
	void checkCube(const glm::vec3& pos);
	void checkFace(int face, const glm::vec3& pos);
	void setupMesh();
};
