#include "chunk.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/ext/vector_int3.hpp"
#include "render/shader.hpp"
#include "render/texture_2d_array.hpp"

#include <cstdint>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdexcept>

Chunk::Chunk(Shader& shader, Texture2DArray& tex, const glm::vec3& worldPos)
    : _shader(shader),
      _tex(tex),
      _worldPos(worldPos),
      _model(glm::translate(glm::mat4(1.0f), _worldPos))
{
	_vertices.reserve(kChunkWidth * kChunkHeight);
	_indices.reserve(kChunkWidth * kChunkWidth * 6);

	_voxels.fill(kBlockNone);
	for (int z = 0; z < kChunkWidth; z++)
		for (int y = 0; y < kChunkHeight; y++)
			for (int x = 0; x < kChunkWidth; x++)
				index({x, y, z}) =
				    (x % 2 == y % 2) == z % 2 ? kBlockStone : kBlockOakPlanks;
}

void Chunk::build()
{
	for (int z = 0; z < kChunkWidth; z++)
		for (int y = 0; y < kChunkHeight; y++)
			for (int x = 0; x < kChunkWidth; x++)
				checkCube(glm::ivec3(x, y, z));

	setupMesh();
}

constexpr unsigned int kQuadIndices[6] = {0, 1, 2, 0, 2, 3};

void Chunk::checkCube(const glm::ivec3& pos)
{
	if (index(pos) != kBlockNone)
		for (uint8_t face = kFaceRight; face < kFaceCount; face++)
			checkFace(face, pos);
}

constexpr glm::ivec3 kNeighbours[6] = {
    glm::ivec3(1, 0, 0),  glm::ivec3(-1, 0, 0), glm::ivec3(0, 1, 0),
    glm::ivec3(0, -1, 0), glm::ivec3(0, 0, 1),  glm::ivec3(0, 0, -1)};

void Chunk::checkFace(uint8_t face, const glm::ivec3& pos)
{
	glm::ivec3 neighbour = pos + kNeighbours[face];

	if (!isValid(neighbour) || !isBlock(neighbour))
	{
		const unsigned int base = static_cast<unsigned int>(_vertices.size());
		const BlockId      id = index(pos);

		for (uint8_t corner = 0; corner < 4; corner++)
			_vertices.push_back({pos, face, corner, id});
		for (int j = 0; j < 6; j++)
			_indices.push_back(base + kQuadIndices[j]);
	}
}

void Chunk::draw()
{
	_shader.use();
	_shader.setMat4("model", _model);
	_tex.bind();
	glBindVertexArray(_vao);
	glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
}

BlockId& Chunk::index(const glm::ivec3& pos)
{
	if (!isValid(pos))
		throw std::runtime_error("Chunk: position out of chunk");
	return _voxels[pos.x + pos.y * kChunkWidth +
	               pos.z * kChunkWidth * kChunkHeight];
}

BlockId Chunk::index(const glm::ivec3& pos) const
{
	if (!isValid(pos))
		return kBlockNone;
	return _voxels[pos.x + pos.y * kChunkWidth +
	               pos.z * kChunkWidth * kChunkHeight];
}

bool Chunk::isValid(const glm::ivec3& pos) const
{
	if (pos.x < 0 || pos.y < 0 || pos.z < 0 || pos.x >= kChunkWidth ||
	    pos.y >= kChunkHeight || pos.z >= kChunkWidth)
		return false;
	return true;
}

bool Chunk::isBlock(const glm::ivec3& pos) const
{
	if (index(pos) != kBlockNone)
		return true;
	return false;
}

void Chunk::setupMesh()
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
