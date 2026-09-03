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
	uint8_t   id;
};

enum BlockId : uint8_t
{
	kBlockStone,
	kBlockDirt,
	kBlockNone = 255
};

enum Face : uint8_t
{
	kFaceRight,
	kFaceLeft,
	kFaceUp,
	kFaceDown,
	kFaceFront,
	kFaceBack,
	kFaceCount
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

private:
	void checkCube(const glm::ivec3& pos);
	void checkFace(uint8_t face, const glm::ivec3& pos);
	void setupMesh();

	BlockId& index(const glm::ivec3& pos);
	BlockId  index(const glm::ivec3& pos) const;

	bool isValid(const glm::ivec3& pos) const;
	bool isBlock(const glm::ivec3& pos) const;
};
