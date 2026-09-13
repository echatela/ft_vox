#pragma once

#include <array>
#include <cstdint>
#include <vector>

#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_int2.hpp"
#include "glm/ext/vector_int3.hpp"
#include "glm/fwd.hpp"
#include "scene/material.hpp"
#include "scene/node.hpp"

constexpr int kChunkWidth = 16;
constexpr int kChunkHeight = 256;
constexpr int kChunkSize = kChunkWidth * kChunkWidth * kChunkHeight;

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
	kBlockCobblestone,
	kBlockOakPlanks,
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

class Chunk : public Node
{
	glm::ivec2                      _pos;
	std::array<BlockId, kChunkSize> _blocks;
	Material                        _material;

	// TODO: Mesh3D
	unsigned int _vao;
	unsigned int _vbo;
	unsigned int _ebo;

	std::vector<Vertex>       _vertices;
	std::vector<unsigned int> _indices;

	void _draw(RenderContext &context) const;

public:
	Chunk(const glm::ivec2& pos = {0, 0},
	      const Material&   mat = {nullptr, nullptr});

	Chunk(const Chunk& src);
	Chunk& operator=(const Chunk& rhs);

	void buildMesh();

	BlockId& at(const glm::ivec3& pos);
	BlockId  at(const glm::ivec3& pos) const;

private:
	void _load();

	void _buildCube(const glm::ivec3& pos);
	void _buildFace(uint8_t face, const glm::ivec3& pos);
	void _setupMesh();

	bool _isValid(const glm::ivec3& pos) const;
	bool _isBlock(const glm::ivec3& pos) const;

	int _index(const glm::ivec3& pos) const;
};
