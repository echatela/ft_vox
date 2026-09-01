#include "chunk.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "render/shader.hpp"

#include <cstdint>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdexcept>

Chunk::Chunk(const glm::vec3& worldPos)
    : _worldPos(worldPos),
      _model(1.0f),
      _vertices(kChunkWidth * kChunkWidth),
      _indices(kChunkWidth * kChunkWidth * 6)
{
	_model = glm::translate(_model, _worldPos);

	_voxels.fill(kBlockNone);
	for (int y = 0; y < kChunkHeight / 2; y++)
		for (int x = 0; x < kChunkWidth; x++)
			for (int z = 0; z < kChunkWidth; z++)
				index(x, y, z) = kBlockStone;
}

void Chunk::build()
{
	for (int z = 0; z < kChunkWidth; z++)
		for (int y = 0; y < kChunkHeight; y++)
			for (int x = 0; x < kChunkWidth; x++)
				checkCube(x, y, z);

	setupMesh();
}

constexpr unsigned int kQuadIndices[6] = {0, 1, 2, 0, 2, 3};

void Chunk::checkCube(int x, int y, int z)
{
	if (index(x, y, z) != kBlockNone)
		for (uint8_t face = kFaceRight; face < kFaceCount; face++)
			checkFace(face, x, y, z);
}

void Chunk::checkFace(uint8_t face, int x, int y, int z)
{
	int cx = x;
	int cy = y;
	int cz = z;

	switch (face)
	{
	case kFaceRight:
		cx++;
		break;
	case kFaceLeft:
		cx--;
		break;
	case kFaceUp:
		cy++;
		break;
	case kFaceDown:
		cy--;
		break;
	case kFaceFront:
		cz++;
		break;
	case kFaceBack:
		cz--;
		break;
	}

	if (!isValid(cx, cy, cz) || !isBlock(cx, cy, cz))
	{
		const unsigned int base = static_cast<unsigned int>(_vertices.size());
		glm::vec3          pos(x, y, z);

		for (uint8_t corner = 0; corner < 4; corner++)
			_vertices.push_back({pos, face, corner});
		for (int j = 0; j < 6; j++)
			_indices.push_back(base + kQuadIndices[j]);
	}
}

void Chunk::draw(Shader& shader)
{
	shader.use();
	shader.setMat4("model", _model);
	glBindVertexArray(_vao);
	glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
}

BlockId& Chunk::index(int x, int y, int z)
{
	if (!isValid(x, y, z))
		throw std::runtime_error("Chunk: position out of chunk");
	return _voxels[x + y * kChunkWidth + z * kChunkWidth * kChunkHeight];
}

BlockId Chunk::index(int x, int y, int z) const
{
	if (!isValid(x, y, z))
		return kBlockNone;
	return _voxels[x + y * kChunkWidth + z * kChunkWidth * kChunkHeight];
}

bool Chunk::isValid(int x, int y, int z) const
{
	if (x < 0 || y < 0 || z < 0 || x >= kChunkWidth || y >= kChunkHeight ||
	    z >= kChunkWidth)
		return false;
	return true;
}

bool Chunk::isBlock(int x, int y, int z) const
{
	if (index(x, y, z) != kBlockNone)
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
	// iteration
	glVertexAttribIPointer(2, 1, GL_UNSIGNED_BYTE, sizeof(Vertex),
	                       (void*)offsetof(Vertex, corner));
	glEnableVertexAttribArray(2);
}
