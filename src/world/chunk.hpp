#pragma once

#include "glm/ext/matrix_float4x4.hpp"
#include "glm/fwd.hpp"
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

	glm::vec3 _worldPos;
	glm::mat4 _model;

	unsigned int _vao;
	unsigned int _vbo;
	unsigned int _ebo;

	std::vector<Vertex>       _vertices;
	std::vector<unsigned int> _indices;

public:
	Chunk(const glm::vec3& worldPos = glm::vec3(0, 0, 0));

	void build();
	void draw(Shader& shader);
	// bool at(int x, int y, int z) const;

private:
	void checkCube(const glm::vec3& pos);
	void checkFace(uint8_t face, const glm::vec3& pos);
	void setupMesh();
};
