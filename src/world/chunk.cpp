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

#define INDEX(pos) ((pos.y * kChunkWidth + pos.z) * kChunkWidth + pos.x)
#define AT(pos) _blocks[INDEX(pos)]
// #define BOOL_AT(pos) (_bitBlock[INDEX(pos) / 8])
#define VALID(pos) (pos.x >= 0 && pos.y >= 0 && pos.z >= 0 && pos.x < kChunkWidth && pos.y < kChunkHeight && pos.z < kChunkWidth)

// #define IS_EDGE(i) (i & 0b1111 == 0 || i & 0b1111 == 0b1111 || i & 0b11110000 == 0b11110000 || i & 0b11110000 == 0b00000000) 
	#define IS_EDGE(i) (((i & 0xF) == 0) || ((i & 0xF) == 0xF) || (((i >> 4) & 0xF) == 0) || (((i >> 4) & 0xF) == 0xF) || (((i >> 8) & 0xFF) == 0) || (((i >> 8) & 0xFF) == 0xFF))

#define FACE_EDGE(i) ((((i & 0xF) == 0) << kFaceLeft) +\
 					 (((i & 0xF) == 0xF) << kFaceRight ) +\
					 ((((i >> 4) & 0xF) == 0) << kFaceBack) +\
					 ((((i >> 4) & 0xF) == 0xF) << kFaceFront) +\
					 ((((i >> 8) & 0xFF) == 0) << kFaceDown) +\
					 ((((i >> 8) & 0xFF) == 0xFF) << kFaceUp))


//  yyyy yyyy zzzz xxxx

// typedef struct chunkIndex
// {
// 	uint16_t	x : 4;
// 	uint16_t	z : 4;
// 	uint16_t	y : 8;
// }	chunkIndex;

Chunk::Chunk()
{
}

Chunk::Chunk(const glm::ivec2& pos, const Material& mat)
    : _pos(pos),
      _material(mat)
{
	_load();
	buildMesh();
}

void Chunk::rebuild(const glm::ivec2 &pos)
{
	_pos = pos;
	_vertices.clear();
	_indices.clear();

	glDeleteVertexArrays(1, &_vao);
	glDeleteBuffers(1, &_vbo);
	glDeleteBuffers(1, &_ebo);

	_load();
	buildMesh();
}


//NOTE : do we really use that constructor ?
Chunk::Chunk(const Chunk& src)
    : _pos(src._pos),
      _material(src._material)
{
	// _ID = src.getID();

	_pos = src._pos;
	_material = src._material;
	
	_vertices = src._vertices;
	_indices = src._indices;
	// _load();
	// buildMesh();
}

// TODO : this just functions because its the same chunk
Chunk& Chunk::operator=(const Chunk& rhs)
{
	if (this != &rhs)
	{
		_pos = rhs._pos;
		_material = rhs._material;
		
		std::cout << "Chunk::operator=" << std::endl;
		buildMesh();
		// _vertices = rhs._vertices;
		// _indices = rhs._indices;
	}
	return *this;
}

// hard coded generation, will need to implement a seed based generation
void Chunk::_load()
{
	_blocks.fill(kBlockNone);
	_bitBlocks.reset();

	glm::ivec3 vec = {0, 0, 0};

	uint16_t i = 0;
	// int y = 0;
	for (vec.y = 0; vec.y < 64; vec.y++)
	{
		for (vec.z = 0; vec.z < kChunkWidth; vec.z++)
		{
			for (vec.x = 0; vec.x < kChunkWidth; vec.x++)
			{
				AT(vec) = kBlockStone;

				_blocks[i] = kBlockStone;
				_bitBlocks.set(i, true);
				i++;
				// BOOL_AT(vec) = true;
			}
		}
	}
	for (; vec.y < 64 + 16; vec.y++)
	{
		for (vec.z = 0; vec.z < kChunkWidth; vec.z++)
		{
			for (vec.x = 0; vec.x < kChunkWidth; vec.x++)
			{
				_blocks[i] = kBlockDirt;
				_bitBlocks.set(i, true);
				i++;
				// AT(vec) = kBlockDirt;
				// _bitBlocks.set(INDEX(vec), true);
			}
		}
	}
}

void Chunk::buildMesh()
{
	// Need to check if these values are fixed or can become higher
	_vertices.reserve(30000);
	_indices.reserve(40000);

	for (unsigned int i = 0; i < kChunkSize; i++)
	{
		if (_bitBlocks[i])
			_buildCube(i);
	}
	_vertices.shrink_to_fit();
	_indices.shrink_to_fit();
	_setupMesh();
}

constexpr unsigned int kQuadIndices[6] = {0, 1, 2, 0, 2, 3};

// constexpr glm::ivec3 kNeighbours[6] = {
//     glm::ivec3(1, 0, 0),  glm::ivec3(-1, 0, 0), glm::ivec3(0, 1, 0),
//     glm::ivec3(0, -1, 0), glm::ivec3(0, 0, 1),  glm::ivec3(0, 0, -1)};

constexpr int kNeighbourOffset[6] = {
	1, -1,
	kChunkHeight, -kChunkHeight,
	kChunkWidth, -kChunkWidth
};

// pas moyen de faire une fonction recursive ?

void Chunk::_buildCube(uint16_t i)
{

	const BlockId id = _blocks[i]; //AT(pos);
	Vertex        vertex = {i, 0, 0, id};
	uint8_t       isEdge = FACE_EDGE(i);

	for (vertex.face = kFaceRight; vertex.face < kFaceCount; vertex.face++)
	{
		unsigned int  base = _vertices.size();

		unsigned int nIndex = i + kNeighbourOffset[vertex.face];

		if (((isEdge >> vertex.face) & 1) || !_bitBlocks[nIndex])
		{	
			for (vertex.corner = 0; vertex.corner < 4; vertex.corner++)
				_vertices.push_back(vertex);
			for (int j = 0; j < 6; j++)
				_indices.push_back(base + kQuadIndices[j]);
		}
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

void Chunk::setMaterial(const Material& mat)
{
	_material = mat;
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
