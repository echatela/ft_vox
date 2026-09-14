#include "chunk_manager.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_int2_sized.hpp"
#include "loader/resource_manager.hpp"
#include "render/texture_2d_array.hpp"
#include "scene/material.hpp"
#include <exception>

constexpr int kViewDistance = 5;

ChunkManager::ChunkManager()
{
	_material.shader =
	    ResourceManager::instance().get<Shader>(ResourceId::SHADER_CHUNK);
	_material.texture = ResourceManager::instance().get<Texture2DArray>(
	    ResourceId::TEXTURE_BLOCKS);
	loadAround({0, 0, 0});
}

void ChunkManager::loadAround(const glm::ivec3& pos)
{
	int y = pos.z - kViewDistance;
	for (; y <= pos.z + kViewDistance; y++)
	{
		int x = pos.x - kViewDistance;
		for (; x <= pos.x + kViewDistance; x++)
		{
			_loadChunk({x, y});
		}
	}
}

void ChunkManager::draw(glm::mat4 matrix) const
{
	// the const & is really important for performance !!!
	for (const auto& chunkIt : _chunks)
	{
		chunkIt.second.draw(matrix);
	}
}

void ChunkManager::setMaterial(const Material& mat)
{
	_material = mat;
}

const Material& ChunkManager::getMaterial() const
{
	return _material;
}

void ChunkManager::_loadChunk(const glm::i32vec2& pos)
{
	if (!_isLoaded(pos))
		_chunks.insert({pos, Chunk(pos * 16, _material)});
}

bool ChunkManager::_isLoaded(const glm::i32vec2& pos)
{
	try
	{
		_chunks.at(pos);
	}
	catch (const std::exception&)
	{
		return false;
	}
	return true;
}
