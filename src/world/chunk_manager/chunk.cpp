#include "chunk.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_int2.hpp"
#include "glm/ext/vector_int3.hpp"
#include "render/shader.hpp"
#include "scene/material.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdexcept>

#define INDEX(pos) pos.y*kStrideY + pos.z*kStrideZ + pos.x*kStrideX
#define AT(pos) _blocks[INDEX(pos)]
#define VALID(pos)                                                             \
	(pos.x >= 0 && pos.y >= 0 && pos.z >= 0 && pos.x < kChunkWidth &&          \
	 pos.y < kChunkHeight && pos.z < kChunkWidth)

Chunk::Chunk()
{
}

Chunk::Chunk(const glm::ivec2& pos, const Material& mat)
    : _pos(pos),
      _material(mat)
{
}

void Chunk::generate(const glm::ivec2& pos, unsigned int seed)
{
	(void)seed;
	_pos = pos;

	int i;

	_blocks.fill(kBlockNone);
	_bitBlocks.reset();

	for (i = 0; i < kChunkSize / 2; i++)
	{
		_blocks[i] = kBlockStone;
		_bitBlocks.set(i, true);
	}
	for (i = kChunkSize / 2; i < kChunkSize / 2 + kChunkWidth * kChunkWidth * 6;
	     i++)
	{
		_blocks[i] = kBlockDirt;
		_bitBlocks.set(i, true);
	}
}

void Chunk::clear()
{
	_pos = {0, 0};
	_vertices.clear();
	_indices.clear();
	glDeleteVertexArrays(1, &_vao);
	glDeleteBuffers(1, &_vbo);
	glDeleteBuffers(1, &_ebo);
}

void Chunk::_draw(RenderContext& context) const
{
	glBindVertexArray(_vao);

	context.mat3D *= glm::translate(
	    glm::mat4(1.0f), {_pos.x * kChunkWidth, 0, _pos.y * kChunkWidth});
	_material.shader->use();
	_material.shader->setUniform<const glm::mat4x4&>("matrix", context.mat3D);
	_material.texture->bind(0);
	_material.shader->setUniform<int>("uBlocksTexture", 0);

	glEnable(GL_DEPTH_TEST);
	glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
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
	glVertexAttribIPointer(0, 1, GL_UNSIGNED_SHORT, sizeof(Vertex), (void*)0);
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

////////////////////////////////////////////////////////////////////////////////

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

void Chunk::setMaterial(const Material& mat)
{
	_material = mat;
}
