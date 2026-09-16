#include "chunk_manager.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_int2_sized.hpp"
#include "loader/resource_manager.hpp"
#include "render/texture_2d_array.hpp"
#include "scene/material.hpp"
#include <exception>

constexpr int kLoadDistance = 1;
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

constexpr auto kLoadRange = kLoadDistance * 2 + 1;

// Iterate on the new range (to be loaded) and the old range (to delete)
// void ChunkManager::_swapRange(const glm::ivec2& oldPos, const glm::ivec2& newPos)
// {
// 	glm::ivec2 diff = newPos - oldPos;
// 	glm::ivec2 loadPos, unloadPos;

// 	if (diff.x)
// 	{
// 		loadPos = glm::ivec2(newPos.x + diff.x * kLoadDistance, newPos.y - kLoadDistance);
// 		unloadPos = glm::ivec2(oldPos.x - diff.x * kLoadDistance, oldPos.y - kLoadDistance);

// 		for (unsigned int i = 0; i < kLoadRange; i++)
// 		{
// 			_swapChunk(unloadPos, loadPos);

// 			loadPos.y ++;
// 			unloadPos.y ++;
// 		}
// 	}
// 	if (diff.y)
// 	{
// 		loadPos = glm::ivec2(newPos.x - kLoadDistance, newPos.y + diff.y * kLoadDistance );
// 		unloadPos = glm::ivec2(oldPos.x - kLoadDistance, oldPos.y - diff.y * kLoadDistance );
		
// 		for (unsigned int i = 0; i < kLoadRange; i++)
// 		{
// 			_swapChunk(unloadPos, loadPos);

// 			loadPos.x ++;
// 			unloadPos.x ++;
// 		}
// 	}
// }

void ChunkManager::_swapRange(const glm::ivec2& oldPos, const glm::ivec2& newPos)
{
	glm::ivec2 diff = newPos - oldPos;
	glm::ivec2 loadPos, unloadPos;

	// if (std::abs(diff.x) >= kLoadRange || std::abs(diff.y) >= kLoadRange)	// If outside of old range, relload everything
	// {
	// 	for (int dX = -kLoadDistance; dX <= kLoadDistance; ++dX) {
	// 		unloadPos.x = oldPos.x + dX;
	// 		loadPos.x = newPos.x + dX;
	// 		for (int dY = -kLoadDistance; dY <= kLoadDistance; ++dY) {
	// 			loadPos.y = newPos.y + dY;
	// 			unloadPos.x = oldPos.x + dX;
	// 			_swapChunk(unloadPos, loadPos);
	// 		}
	// 	}
	// } 
	
	// else
	{
		const int		signX = diff.x >= 0 ? 1 : -1;
		const int		signY = diff.y >= 0 ? 1 : -1;

		const size_t	rA_dX = std::abs(diff.x);
		// const size_t	rA_dY = kLoadRange;

		const size_t	rB_dX = kLoadRange - rA_dX;
		const size_t	rB_dY = std::abs(diff.y);

		// Iter over A rectangle
		for (uint dX = 0; dX < rA_dX; ++dX) // pour chaque ligne x en plus
		{
			const int rX = kLoadDistance - dX;
			loadPos.x = newPos.x + rX * signX ;
			unloadPos.x = oldPos.x + rX * -signX;
			for (uint dY = 0; dY < kLoadRange; ++dY) // on itere sur l'entierete des y
			{
				const int rY = kLoadDistance - dY;
				loadPos.y = newPos.y - rY;
				unloadPos.y = oldPos.y - rY;
				// _swapChunk(unloadPos, loadPos);
				_loadChunk(loadPos);
				_unloadChunk(unloadPos);
			}
		}
		// Iter over B rectangle
		for (uint dY = 0; dY < rB_dY; ++dY) // pour chaque ligne x en plus
		{
			const int rY = kLoadDistance - dY;
			loadPos.y = newPos.y + rY * signY;
			unloadPos.y = oldPos.y + rY * -signY;
			for (uint dX = 0; dX < rB_dX; ++dX) // on itere sur l'entierete des y
			{
				const int rX = kLoadDistance - dX;
				loadPos.x = newPos.x - rX;
				unloadPos.x = oldPos.x - rX;
				// _swapChunk(unloadPos, loadPos);
				_loadChunk(loadPos);
				_unloadChunk(unloadPos);
			}
		}
	}
}

// Load + Unload a chunk, keeping the old allocated Chunk pointer
// No need to remove from _tree because it uses the Chunk*
void ChunkManager::_swapChunk(const glm::ivec2& oldPos, const glm::ivec2& newPos)
{
	auto it = _chunks.find(oldPos);

	if (it == _chunks.end())
		return;

	//reuse allocated chunk
	*(it->second) = Chunk(newPos * 16, _material);

	//insert new pair & remove old
	_chunks[newPos] = it->second;
	_chunks.erase(it);

}

void ChunkManager::updateChunks(const glm::vec3 &pos, bool preload)
{
	static glm::ivec2 lastPos;

	glm::ivec2 newPos = glm::ivec2{std::floor(pos.x / 16), std::floor(pos.z / 16)};

	if (preload)
	{
		_chunks.reserve(kLoadDistance * kLoadDistance + 1);
		_loadAround(newPos);
	}

	else if (newPos != lastPos)
	{
		_swapRange(lastPos, newPos);
	}
	
	lastPos = newPos;

}

void ChunkManager::_loadAround(const glm::ivec2& pos)
{
	glm::ivec2 vec = {0, 0};

	for (vec.y = pos.y - kLoadDistance; vec.y <= pos.y + kLoadDistance; vec.y++)
	{
		for (vec.x = pos.x - kLoadDistance; vec.x <= pos.x + kLoadDistance; vec.x++)
		{
			_loadChunk(vec);
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

void ChunkManager::_unloadChunk(const glm::i32vec2& pos)
{

	if (_isLoaded(pos))
	{
		Chunk* ptr = _chunks[pos];

		_chunks.extract(pos);
		_tree.extract("chunk" + std::to_string(ptr->getID()));
		delete ptr;

	}
	else
	{
		std::cerr << pos.x << ", " << pos.y << std::endl;
		std::cerr << "Tried to access unloaded chunk boyyy" << std::endl;
	}
}

// void ChunkManager::_loadAndReplaceChunk(const glm::i32vec2& pos, chunk_it spot)
// {
// 	*(spot->second) = Chunk(pos * 16, _material);

// 	auto pair = _chunks.extract(spot->first);

// 	pair.key() = pos;
// 	_chunks.insert(std::move(pair));
// }


bool ChunkManager::_isLoaded(const glm::i32vec2& pos)
{
	return (_chunks.find(pos) != _chunks.end());
}
