#include "render/mesh.hpp"
#include "data/vertex.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

Mesh::Mesh(const std::vector<Vertex>&       vertices,
           const std::vector<unsigned int>& indices)
    : _vertices(vertices),
      _indices(indices)
{
	setupMesh();
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &_vao);
	glDeleteBuffers(1, &_vbo);
	glDeleteBuffers(1, &_ebo);
}

void Mesh::draw()
{
	glBindVertexArray(_vao);
	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)_vertices.size());
}

////////////////////////////////////////////////////////////////////////////////

constexpr Vertex kFaceUp[4] = {
    {glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
    {glm::vec3(1.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
    {glm::vec3(1.0f, 1.0f, -1.0f), glm::vec2(1.0f, 1.0f)},
    {glm::vec3(0.0f, 1.0f, -1.0f), glm::vec2(0.0f, 1.0f)}};

constexpr Vertex kFaceDown[4] = {
    {glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
    {glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f)},
    {glm::vec3(1.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f)},
    {glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)}};

constexpr Vertex kFaceRight[4] = {
    {glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
    {glm::vec3(1.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
    {glm::vec3(1.0f, 1.0f, -1.0f), glm::vec2(1.0f, 1.0f)},
    {glm::vec3(0.0f, 1.0f, -1.0f), glm::vec2(0.0f, 1.0f)}};

constexpr Vertex kFaceLeft[4] = {
    {glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
    {glm::vec3(1.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
    {glm::vec3(1.0f, 1.0f, -1.0f), glm::vec2(1.0f, 1.0f)},
    {glm::vec3(0.0f, 1.0f, -1.0f), glm::vec2(0.0f, 1.0f)}};

constexpr Vertex kFaceForward[4] = {
constexpr Vertex kFaceBackwar[4] = {

void Mesh::setupMesh()
{
	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);

	glBindVertexArray(_vao);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);

	glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex),
	             &_vertices[0], GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
	                      (void*)offsetof(Vertex, texCoord));

	glBindVertexArray(0);
}
