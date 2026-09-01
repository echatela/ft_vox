#include "chunk.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "render/shader.hpp"

#include <cstdint>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

Chunk::Chunk(const glm::vec3& worldPos)
    : _worldPos(worldPos),
      _model(1.0f),
      _vertices(kChunkWidth * kChunkWidth),
      _indices(kChunkWidth * kChunkWidth * 6)
{
	_model = glm::translate(_model, _worldPos);
	_voxels.fill(kBlockStone);
}

void Chunk::build()
{
	glm::vec3 pos;

	for (int i = 0; i < kChunkSize; i++)
	{
		pos.x = i % kChunkWidth;
		pos.y = i / kChunkWidth % kChunkHeight;
		pos.z = i / (kChunkWidth * kChunkHeight) % kChunkWidth;

		checkCube(pos);
	}
	setupMesh();
}

// TODO : could put x / y / z out of loop to make less calcul
void Chunk::draw(Shader& shader)
{
	shader.use();
	shader.setMat4("model", _model);
	glBindVertexArray(_vao);
	glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
}

enum Face : uint8_t
{
	kFaceRight,
	kFaceLeft,
	kFaceUp,
	kFaceDown,
	kFaceForward,
	kFaceBack,
	kFaceCount
};

constexpr unsigned int kQuadIndices[6] = {0, 1, 2, 0, 2, 3};

void Chunk::checkCube(const glm::vec3& pos)
{
	for (uint8_t face = 0; face < kFaceCount; face++)
	{
		checkFace(face, pos);
	}
}

void Chunk::checkFace(uint8_t face, const glm::vec3& pos)
{
	const unsigned int base = static_cast<unsigned int>(_vertices.size());

	for (uint8_t i = 0; i < 4; i++)
		_vertices.push_back({pos, face, i});
	for (int j = 0; j < 6; j++)
		_indices.push_back(base + kQuadIndices[j]);
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
