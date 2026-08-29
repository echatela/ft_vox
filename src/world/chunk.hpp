#pragma once

#include "render/shader.hpp"
static constexpr int kChunkWidth = 16;
static constexpr int kChunkHeight = 256;
static constexpr int kChunkSize = kChunkWidth * kChunkHeight * kChunkWidth;

class Chunk
{
	bool         _voxels[kChunkSize]{true};
	unsigned int _vao;

public:
	Chunk();

	void draw(Shader& shader) const;
	bool at(int x, int y, int z) const;

private:
	void drawCube(Shader& shader, int i) const;
	void setupMesh();
};
