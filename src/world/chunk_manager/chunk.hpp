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

// memory layout : index = y*kStrideY + z*kStrideZ + x*kStrideX
// bits          : [y:8][z:4][x:4]
constexpr int kStrideX = 1;
constexpr int kStrideZ = kChunkWidth;
constexpr int kStrideY = kChunkWidth * kChunkWidth;

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
	kFaceRight, // +X
	kFaceLeft,  // -X
	kFaceUp,    // +Y
	kFaceDown,  // -Y
	kFaceFront, // +Z
	kFaceBack,  // -Z
	kFaceCount
};

class ChunkMesher;

class Chunk : public Node
{
	unsigned int                    _ID = -1;
	glm::ivec2                      _pos = {0, 0};
	std::bitset<kChunkSize>         _bitBlocks{false};
	std::array<BlockId, kChunkSize> _blocks{kBlockNone};
	Material                        _material = {nullptr, nullptr};

	// TODO: Mesh3D
	unsigned int _vao = 0;
	unsigned int _vbo = 0;
	unsigned int _ebo = 0;

	std::vector<Vertex>       _vertices;
	std::vector<unsigned int> _indices;

public:
	Chunk();

	Chunk(const glm::ivec2& pos, const Material& mat);

	Chunk(const Chunk& src);
	Chunk& operator=(const Chunk& rhs);

	void generate(const glm::ivec2& pos, unsigned int seed);
	void clear();

	BlockId& at(const glm::ivec3& pos);
	BlockId  at(const glm::ivec3& pos) const;

	unsigned int getID() const;
	void         setID(unsigned int ID);

	void setMaterial(const Material& mat);

private:
	void _draw(RenderContext& context) const;

	void _buildCube(uint16_t i);
	void _buildFace(uint8_t face, const glm::ivec3& pos);
	void _setupMesh();

	bool _isValid(const glm::ivec3& pos) const;
	bool _isBlock(const glm::ivec3& pos) const;

	int _index(const glm::ivec3& pos) const;

	friend ChunkMesher;
};
