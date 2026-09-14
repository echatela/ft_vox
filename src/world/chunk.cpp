#include "chunk.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_int2.hpp"
#include "glm/ext/vector_int3.hpp"
#include "render/shader.hpp"
#include "scene/material.hpp"

#include <cstdint>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdexcept>

Chunk::Chunk(const glm::ivec2& pos, const Material& mat)
    : _pos(pos),
      _material(mat)
{
	_load();
	buildMesh();
}

Chunk::Chunk(const Chunk& src)
    : _pos(src._pos),
      _material(src._material)
{
	_load();
	buildMesh();
}

// TODO: Probably a bad thing to do, maybe the mesh is created and rebuild at
// copy
Chunk& Chunk::operator=(const Chunk& rhs)
{
	if (this != &rhs)
	{
		_pos = rhs._pos;
		_material = rhs._material;
	}
	return *this;
}

// hard coded generation, will need to implement a seed based generation
void Chunk::_load()
{
	_blocks.fill(kBlockNone);
	int y = 0;
	for (; y < 64; y++)
	{
		for (int z = 0; z < kChunkWidth; z++)
		{
			for (int x = 0; x < kChunkWidth; x++)
				at({x, y, z}) = kBlockStone;
		}
	}
	for (; y < 64 + 16; y++)
	{
		for (int z = 0; z < kChunkWidth; z++)
		{
			for (int x = 0; x < kChunkWidth; x++)
				at({x, y, z}) = kBlockDirt;
		}
	}
}

void Chunk::buildMesh()
{
	for (int y = 0; y < kChunkHeight; y++)
	{
		for (int z = 0; z < kChunkWidth; z++)
		{
			for (int x = 0; x < kChunkWidth; x++)
				_buildCube(glm::ivec3(x, y, z));
		}
	}
	_setupMesh();
}

void Chunk::draw(glm::mat4 matrix) const
{
	matrix *= glm::translate(glm::mat4(1.0f), {_pos.x, 0, _pos.y});
	_material.shader->use();
	_material.shader->setUniform<const glm::mat4x4&>("matrix", matrix);
	_material.texture->bind(0);
	_material.shader->setUniform<int>("uBlocksTexture", 0);

	glBindVertexArray(_vao);
	glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
}

void Chunk::_buildCube(const glm::ivec3& pos)
{
	if (at(pos) != kBlockNone)
	{
		for (uint8_t face = kFaceRight; face < kFaceCount; face++)
			_buildFace(face, pos);
	}
}

constexpr unsigned int kQuadIndices[6] = {0, 1, 2, 0, 2, 3};

constexpr glm::ivec3 kNeighbours[6] = {
    glm::ivec3(1, 0, 0),  glm::ivec3(-1, 0, 0), glm::ivec3(0, 1, 0),
    glm::ivec3(0, -1, 0), glm::ivec3(0, 0, 1),  glm::ivec3(0, 0, -1)};

void Chunk::_buildFace(uint8_t face, const glm::ivec3& pos)
{
	glm::ivec3 neighbour = pos + kNeighbours[face];

	if (!_isValid(neighbour) || !_isBlock(neighbour))
	{
		const unsigned int base = static_cast<unsigned int>(_vertices.size());
		const BlockId      id = at(pos);

		for (uint8_t corner = 0; corner < 4; corner++)
			_vertices.push_back({pos, face, corner, id});
		for (int j = 0; j < 6; j++)
			_indices.push_back(base + kQuadIndices[j]);
	}
}

int Chunk::_index(const glm::ivec3& pos) const
{
	return (pos.y * kChunkWidth + pos.z) * kChunkWidth + pos.x;
}

BlockId& Chunk::at(const glm::ivec3& pos)
{
	if (!_isValid(pos))
		throw std::runtime_error("Chunk: position out of chunk");
	return _blocks[_index(pos)];
}

BlockId Chunk::at(const glm::ivec3& pos) const
{
	if (!_isValid(pos))
		return kBlockNone;
	return _blocks[_index(pos)];
}

bool Chunk::_isValid(const glm::ivec3& pos) const
{
	if (pos.x < 0 || pos.y < 0 || pos.z < 0 || pos.x >= kChunkWidth ||
	    pos.y >= kChunkHeight || pos.z >= kChunkWidth)
		return false;
	return true;
}

bool Chunk::_isBlock(const glm::ivec3& pos) const
{
	if (at(pos) != kBlockNone)
		return true;
	return false;
}

void Chunk::_setupMesh()
{
	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);
	glGenBuffers(1, &_ebo);

	glBindVertexArray(_vao);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex),
	             _vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
	             _indices.size() * sizeof(unsigned int), _indices.data(),
	             GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);
	// face attribute
	glVertexAttribIPointer(1, 1, GL_UNSIGNED_BYTE, sizeof(Vertex),
	                       (void*)offsetof(Vertex, face));
	glEnableVertexAttribArray(1);
	// corner attribute
	glVertexAttribIPointer(2, 1, GL_UNSIGNED_BYTE, sizeof(Vertex),
	                       (void*)offsetof(Vertex, corner));
	glEnableVertexAttribArray(2);
	// id attribute
	glVertexAttribIPointer(3, 1, GL_UNSIGNED_BYTE, sizeof(Vertex),
	                       (void*)offsetof(Vertex, id));
	glEnableVertexAttribArray(3);
}
