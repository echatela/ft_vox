#include "chunk.hpp"
#include "glm/ext/vector_float3.hpp"
#include "render/shader.hpp"

#include <cstdint>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

Chunk::Chunk()
{
	setupMesh();
}

// TODO : could put x / y / z out of loop to make less calcul
void Chunk::draw(Shader& shader)
{
	glBindVertexArray(_vao);
	glm::vec3 pos;
	for (int i = 0; i < kChunkSize; i++)
	{
		pos.x = i % kChunkWidth;
		pos.y = i / kChunkWidth % kChunkHeight;
		pos.z = i / (kChunkWidth * kChunkHeight) % kChunkWidth;

		checkCube(pos);
	}
	shader.use();
	glDrawArrays(GL_QUADS, 0, 36);
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

void Chunk::checkCube(const glm::vec3& pos)
{
	for (uint8_t face = 0; face < kFaceCount; face++)
	{
		checkFace(face, pos);
	}
}

void Chunk::checkFace(uint8_t face, const glm::vec3& pos)
{
	for (uint8_t i = 0; i < 4; i++)
		_vertices.push_back({pos, face, i});
}

void Chunk::setupMesh()
{
	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);

	glBindVertexArray(_vao);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex),
	             _vertices.data(), GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);
	// face attribute
	glVertexAttribIPointer(1, 1, GL_UNSIGNED_INT, sizeof(Vertex),
	                       (void*)offsetof(Vertex, face));
	glEnableVertexAttribArray(1);
	// iteration
	glVertexAttribIPointer(2, 1, GL_UNSIGNED_INT, sizeof(Vertex),
	                       (void*)offsetof(Vertex, corner));
	glEnableVertexAttribArray(2);
}
