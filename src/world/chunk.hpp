#pragma once

#include <array>
#include <cstdint>
#include <vector>
#include <bitset>

#include "glm/ext/vector_int2.hpp"
#include "glm/ext/vector_int3.hpp"
#include "glm/fwd.hpp"
#include "scene/material.hpp"
#include "scene/node.hpp"

constexpr int kChunkWidth = 16;
constexpr int kChunkHeight = 256;
constexpr int kChunkSize = kChunkWidth * kChunkWidth * kChunkHeight;
constexpr int kChunkBytes = kChunkSize / sizeof(uint8_t);

struct Vertex
{
	uint16_t pos;
	uint8_t  face;
	uint8_t  corner;
	uint8_t  id;
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
	unsigned int                    _ID;
	glm::ivec2                      _pos;
	std::bitset<kChunkSize>         _bitBlocks;
	std::array<BlockId, kChunkSize> _blocks;
	Material                        _material;

	// TODO: Mesh3D
	unsigned int _vao;
	unsigned int _vbo;
	unsigned int _ebo;

	std::vector<Vertex>       _vertices;
	std::vector<unsigned int> _indices;

	void _draw(RenderContext& context) const;

public:
	Chunk();

	Chunk(const glm::ivec2& pos, const Material& mat);

	Chunk(const Chunk& src);
	Chunk& operator=(const Chunk& rhs);

	void buildMesh();

	void rebuild(const glm::ivec2& pos);

	BlockId& at(const glm::ivec3& pos);
	BlockId  at(const glm::ivec3& pos) const;

	unsigned int getID() const;
	void         setID(unsigned int ID);

	void setMaterial(const Material& mat);

private:
	void _load();

	void _buildCube(uint16_t i);
	void _buildFace(uint8_t face, const glm::ivec3& pos);
	void _setupMesh();

	bool _isValid(const glm::ivec3& pos) const;
	bool _isBlock(const glm::ivec3& pos) const;

	int _index(const glm::ivec3& pos) const;
};
