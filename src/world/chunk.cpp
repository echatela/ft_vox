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

#define AT(pos) _blocks[(pos.y * kChunkWidth + pos.z) * kChunkWidth + pos.x]
#define VALID(pos) (pos.x >= 0 && pos.y >= 0 && pos.z >= 0 && pos.x < kChunkWidth && pos.y < kChunkHeight && pos.z < kChunkWidth)

Chunk::Chunk(const glm::ivec2& pos, const Material& mat)
    : _pos(pos),
      _material(mat)
{
	_load();
	buildMesh();

}

//NOTE : should the id be kept in this case and the operator= ?
Chunk::Chunk(const Chunk& src)
    : _pos(src._pos),
      _material(src._material)
{
	_ID = src.getID();
	_load();
	buildMesh();
}

// TODO: Probably a bad thing to do, maybe the mesh is created and rebuild at
// copy
Chunk& Chunk::operator=(const Chunk& rhs)
{
	if (this != &rhs)
	{
		_ID = rhs.getID();
		_pos = rhs._pos;
		_material = rhs._material;
	}
	return *this;
}

// hard coded generation, will need to implement a seed based generation
void Chunk::_load()
{
	_blocks.fill(kBlockNone);

	glm::ivec3 vec = {0, 0, 0};
	// int y = 0;
	for (vec.y = 0; vec.y < 64; vec.y++)
	{
		for (vec.z = 0; vec.z < kChunkWidth; vec.z++)
		{
			for (vec.x = 0; vec.x < kChunkWidth; vec.x++)
			{
				AT(vec) = kBlockStone;
			}
		}
	}
	for (; vec.y < 64 + 16; vec.y++)
	{
		for (vec.z = 0; vec.z < kChunkWidth; vec.z++)
		{
			for (vec.x = 0; vec.x < kChunkWidth; vec.x++)
				AT(vec) = kBlockDirt;
		}
	}
}

void Chunk::buildMesh()
{
	// Need to check if these values are fixed or can become higher
	_vertices.reserve(30000);
	_indices.reserve(40000);

	glm::ivec3 vec = {0, 0, 0};

	for (vec.y = 0; vec.y < kChunkHeight; vec.y++)
	{
		for (vec.z = 0; vec.z < kChunkWidth; vec.z++)
		{
			for (vec.x = 0; vec.x < kChunkWidth; vec.x++)
				_buildCube(vec);
		}
	}
	_vertices.shrink_to_fit();
	_indices.shrink_to_fit();
	_setupMesh();
}

void Chunk::_buildCube(const glm::ivec3& pos)
{
	if (AT(pos) != kBlockNone)
	{
		for (uint8_t face = kFaceRight; face < kFaceCount; face++)
			_buildFace(face, pos);
	}
}

constexpr unsigned int kQuadIndices[6] = {0, 1, 2, 0, 2, 3};

constexpr glm::ivec3 kNeighbours[6] = {
    glm::ivec3(1, 0, 0),  glm::ivec3(-1, 0, 0), glm::ivec3(0, 1, 0),
    glm::ivec3(0, -1, 0), glm::ivec3(0, 0, 1),  glm::ivec3(0, 0, -1)};

//NOTE : maybe we can precalculate info for the next faces etc ?
void Chunk::_buildFace(uint8_t face, const glm::ivec3& pos)
{
	glm::ivec3 neighbour = pos + kNeighbours[face];

	if (!VALID(neighbour) || !_isBlock(neighbour))
	{
		const unsigned int base = static_cast<unsigned int>(_vertices.size());
		const BlockId      id = AT(pos);

		for (uint8_t corner = 0; corner < 4; corner++)
			_vertices.push_back({pos, face, corner, id});
		for (int j = 0; j < 6; j++)
			_indices.push_back(base + kQuadIndices[j]);
	}
}

void Chunk::_draw(RenderContext& context) const
{
	glBindVertexArray(_vao);

	context.mat3D *= glm::translate(glm::mat4(1.0f), {_pos.x, 0, _pos.y});
	_material.shader->use();
	_material.shader->setUniform<const glm::mat4x4&>("matrix", context.mat3D);
	_material.texture->bind(0);
	_material.shader->setUniform<int>("uBlocksTexture", 0);

	glEnable(GL_DEPTH_TEST);
	glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
}

int Chunk::_index(const glm::ivec3& pos) const
{
	return (pos.y * kChunkWidth + pos.z) * kChunkWidth + pos.x;
}

BlockId& Chunk::at(const glm::ivec3& pos)
{
	if (!VALID(pos))
		throw std::runtime_error("Chunk: position out of chunk");
	return _blocks[_index(pos)];
}

BlockId Chunk::at(const glm::ivec3& pos) const
{
	if (!VALID(pos))
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
	if (AT(pos) != kBlockNone)
		return true;
	return false;
}

unsigned int Chunk::getID() const
{
	return (_ID);
}

void Chunk::setID(unsigned int ID)
{
	_ID = ID;
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
