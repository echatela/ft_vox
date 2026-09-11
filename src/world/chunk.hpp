#pragma once

#include <array>
#include <cstdint>
#include <vector>

#include "glm/ext/matrix_float4x4.hpp"
#include "glm/fwd.hpp"
#include "render/shader.hpp"
#include "scene/material.hpp"

constexpr int kChunkWidth = 16;
constexpr int kChunkHeight = 256;
constexpr int kChunkSize = kChunkWidth * kChunkHeight * kChunkWidth;

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

class Chunk
{
	std::array<BlockId, kChunkSize> _voxels;

	glm::vec3 _worldPos;
	glm::mat4 _model;

	Material _material;

	unsigned int _vao;
	unsigned int _vbo;
	unsigned int _ebo;

	std::vector<Vertex>       _vertices;
	std::vector<unsigned int> _indices;

public:
	Chunk(const glm::vec3& worldPos = glm::vec3(0, 0, 0),
	      const Shader* shader = nullptr, const ATexture* texture = nullptr);

	void build();
	void draw(glm::mat4 matrix) const;

private:
	void _checkCube(const glm::ivec3& pos);
	void _checkFace(uint8_t face, const glm::ivec3& pos);
	void _setupMesh();

	BlockId& _index(const glm::ivec3& pos);
	BlockId  _index(const glm::ivec3& pos) const;

	bool _isValid(const glm::ivec3& pos) const;
	bool _isBlock(const glm::ivec3& pos) const;
};
