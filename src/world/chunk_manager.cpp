#include "chunk_manager.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_int2_sized.hpp"
#include "loader/resource_manager.hpp"
#include "render/texture_2d_array.hpp"
#include "scene/material.hpp"
#include <exception>

constexpr int kLoadDistance = 10;
//constexpr int kViewDistance = 5;

// TODO : Destructor should empty node and free map
ChunkManager::ChunkManager()
{
	_material.shader =
	    ResourceManager::instance().get<Shader>(ResourceId::SHADER_CHUNK);
	_material.texture = ResourceManager::instance().get<Texture2DArray>(
	    ResourceId::TEXTURE_BLOCKS);
	updateChunks({0, 0, 0}, true);
	// loadAround({0, 0, 0});

}

void ChunkManager::updateChunks(const glm::vec3 &pos, bool preload)
{
	static glm::ivec2 lastPos;

	glm::ivec2 chunkPos = glm::ivec2{std::floor(pos.x / 16), std::floor(pos.z / 16)};

	if (chunkPos != lastPos || preload)
	{
		lastPos = chunkPos;

		_unloadFurther(chunkPos);
		_loadAround(chunkPos);
		// _filterRenderedChunks(chunkPos);

		// std::cout << "NEED UPDATE" << std::endl;
	}

}

void ChunkManager::_unloadFurther(const glm::ivec2& pos)
{
	// TODO : I think this is bad
	for (auto it = _chunks.begin(); it != _chunks.end(); )
	{
		const glm::ivec2 chunkPos = it->first;
		const glm::ivec2 dist = glm::ivec2(pos - chunkPos);

		if (abs(dist.x) > kLoadDistance || abs(dist.y) > kLoadDistance)
		{

			// remove from rendered
			std::string id = "chunk" + std::to_string(it->second->getID());

			// std::cout << "ERASED CHUNK " << id << std::endl;

			auto loadedChunk = _tree.find(id);
			if (loadedChunk != _tree.end())
				_tree.erase(loadedChunk);
			
			// remove from loaded
			delete it->second;
			it = _chunks.erase(it);
		}
		else
			it++;
	}
}

// void ChunkManager::_filterRenderedChunks(const glm::ivec2& pos)
// {
	// 
// }

void ChunkManager::_loadAround(const glm::ivec2& pos)
{
	for (int z = pos.y - kLoadDistance; z <= pos.y + kLoadDistance; z++)
	{
		for (int x = pos.x - kLoadDistance; x <= pos.x + kLoadDistance; x++)
		{
			_loadChunk({x, z});
		}
	}
}

void ChunkManager::_draw(RenderContext& context) const
{
	(void)context;
	// the const & is really important for performance !!!
	// for (const auto& chunkIt : _chunks)
	// {
	// 	chunkIt.second._draw(matrix);
	// }
}

void ChunkManager::setMaterial(const Material& mat)
{
	_material = mat;
}

const Material& ChunkManager::getMaterial() const
{
	return _material;
}

unsigned int ChunkManager::getSize() const
{
	return _chunks.size();
}

void ChunkManager::_loadChunk(const glm::i32vec2& pos)
{
	static unsigned int chunkCount = 0;

	if (!_isLoaded(pos))
	{
		Chunk* chunk = new Chunk(pos * 16, _material);
		chunk->setID(chunkCount);
		_chunks.insert({pos, chunk});

		append("chunk" + std::to_string(chunkCount), chunk);

		// std::cout << "LOADED CHUNK " << "chunk" + std::to_string(chunkCount) << std::endl;
		chunkCount++;
	}
}


bool ChunkManager::_isLoaded(const glm::i32vec2& pos)
{
	return (_chunks.find(pos) != _chunks.end());
}
