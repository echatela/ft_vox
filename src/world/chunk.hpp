#pragma once

#include "render/shader.hpp"
#include <array>
#include <cstdint>
#include <vector>
static constexpr int kChunkWidth = 16;
static constexpr int kChunkHeight = 256;
static constexpr int kChunkSize = kChunkWidth * kChunkHeight * kChunkWidth;

struct Vertex
{
	glm::vec3 pos;
	uint8_t   face;
	uint8_t   corner;
};

enum BlockId
{
	kBlockNone,
	kBlockStone,
};

class Chunk
{
	std::array<BlockId, kChunkSize> _voxels;

	unsigned int _vao;
	unsigned int _vbo;

	std::vector<Vertex> _vertices;

public:
	Chunk();

	void draw(Shader& shader);
	void build();
	// bool at(int x, int y, int z) const;

private:
	void checkCube(const glm::vec3& pos);
	void checkFace(uint8_t face, const glm::vec3& pos);
	void setupMesh();
};
