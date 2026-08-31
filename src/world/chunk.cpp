#include "chunk.hpp"
#include "glm/ext/vector_float3.hpp"
#include "render/shader.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

Chunk::Chunk()
{
	setupMesh();
}

// TODO : could put x / y / z out of loop to make less calcul
void Chunk::draw(Shader& shader)
{
	const int yOffset = static_cast<float>(kChunkHeight) / 2;

	glBindVertexArray(_vao);
	glm::vec3 pos;
	for (int i = 0; i < kChunkSize; i++)
	{
		pos.x = i % kChunkWidth;
		pos.y = i / kChunkWidth % kChunkHeight - yOffset;
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

constexpr glm::u8vec3 kCorners[kFaceCount][4] = {
    {{1, 0, 1}, {1, 0, 0}, {1, 1, 0}, {1, 1, 1}}, // +X
    {{0, 0, 0}, {0, 0, 1}, {0, 1, 1}, {0, 1, 0}}, // -X
    {{0, 1, 1}, {1, 1, 1}, {1, 1, 0}, {0, 1, 0}}, // +Y
    {{0, 0, 0}, {1, 0, 0}, {1, 0, 1}, {0, 0, 1}}, // -Y
    {{0, 0, 1}, {1, 0, 1}, {1, 1, 1}, {0, 1, 1}}, // +Z
    {{1, 0, 0}, {0, 0, 0}, {0, 1, 0}, {1, 1, 0}}, // -Z
};

void Chunk::checkCube(const glm::vec3& pos)
{
	for (int face = 0; face < kFaceCount; face++)
	{
		checkFace(face, pos);
	}
}

void Chunk::checkFace(int face, const glm::vec3& pos)
{
	for (int i = 0; i < 4; i++)
		_vertices.push_back({pos, face, i});
}

void Chunk::setupMesh()
{
	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);

	glBindVertexArray(_vao);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices.data(),
	             GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
	                      (void*)0);
	glEnableVertexAttribArray(0);
	// face attribute
	glVertexAttribPointer(1, 1, GL_INT, GL_FALSE, sizeof(Vertex),
	                      (void*)offsetof(Vertex, face));
	glEnableVertexAttribArray(1);
	// iteration
	glVertexAttribPointer(2, 1, GL_INT, GL_FALSE, sizeof(Vertex),
	                      (void*)offsetof(Vertex, i));
	glEnableVertexAttribArray(2);
}
