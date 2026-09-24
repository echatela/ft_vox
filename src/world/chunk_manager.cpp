#include "chunk_manager.hpp"
#include "glm/ext/vector_int2_sized.hpp"
#include "loader/resource_manager.hpp"
#include "render/texture_2d_array.hpp"
#include "scene/material.hpp"

constexpr int  kLoadDistance = 10;
constexpr auto kLoadRange = kLoadDistance * 2 + 1;
constexpr auto kLoadCount = kLoadRange * kLoadRange;

// constexpr int kViewDistance = 5;

// TODO : Destructor should empty node and free map
ChunkManager::ChunkManager()
{
	_material.shader =
	    ResourceManager::instance().get<Shader>(ResourceId::SHADER_CHUNK);
	_material.texture = ResourceManager::instance().get<Texture2DArray>(
	    ResourceId::TEXTURE_BLOCKS);

	_chunkMemory = new Chunk[kLoadCount];
	_chunks.reserve(kLoadCount);
	_loadAround({0, 0});
}

ChunkManager::~ChunkManager()
{
	delete[] _chunkMemory;

	_tree.clear();
}

////////////////////////////////////////////////////////////////////////////////

void ChunkManager::updateChunks(const glm::vec3& pos)
{
	static glm::ivec2 lastPos(0);

	glm::ivec2 newPos = {std::floor(pos.x / 16), std::floor(pos.z / 16)};

	if (newPos != lastPos)
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
		for (vec.x = pos.x - kLoadDistance; vec.x <= pos.x + kLoadDistance;
		     vec.x++)
		{
			_loadChunk(vec);
		}
	}
}

void ChunkManager::_swapRange(const glm::ivec2& oldPos,
                              const glm::ivec2& newPos)
{
	glm::ivec2 move = newPos - oldPos;
	glm::ivec2 loadPos, unloadPos;

	const int xSign = move.x >= 0 ? 1 : -1;
	const int ySign = move.y >= 0 ? 1 : -1;

	unsigned int fullRow = std::min(std::abs(move.y), kLoadRange);

	loadPos =
	    glm::ivec2(newPos.x + -kLoadDistance, newPos.y + (kLoadDistance)*ySign);
	// Processing each full row
	for (uint row = 0; row < fullRow; row++)
	{
		for (; loadPos.x <= newPos.x + kLoadDistance; loadPos.x++)
		{
			unloadPos = oldPos - (loadPos - newPos);
			_swapChunk(unloadPos, loadPos);
		}
		loadPos.y -= ySign;
	}
	// Processing the leftovers
	for (uint yLeft = 0; yLeft < kLoadRange - fullRow; yLeft++)
	{
		loadPos = glm::ivec2(newPos.x + (kLoadDistance)*xSign,
		                     newPos.y - (kLoadDistance - yLeft) * ySign);
		for (uint xLeft = 0; xLeft < (uint)abs(move.x); xLeft++)
		{
			unloadPos = oldPos - (loadPos - newPos);
			_swapChunk(unloadPos, loadPos);
			loadPos.x -= xSign;
		}
	}
}

// Load + Unload a chunk, keeping the old allocated Chunk pointer
// No need to remove from _tree because it uses the Chunk*
void ChunkManager::_swapChunk(const glm::ivec2& oldPos,
                              const glm::ivec2& newPos)
{
	Chunk* chunk = _chunks[oldPos];

	// reuse allocated chunk
	chunk->rebuild(newPos);

	// insert new pair & remove old
	_chunks.erase(oldPos);
	_chunks[newPos] = chunk;
}

void ChunkManager::_loadChunk(const glm::i32vec2& pos)
{
	static unsigned int chunkCount = 0;

	if (!_isLoaded(pos))
	{
		Chunk* chunk = &(_chunkMemory[chunkCount]);

		chunk->setMaterial(_material);
		chunk->setID(chunkCount);

		chunk->rebuild(pos);
		_chunks.insert({pos, chunk});

		append("chunk" + std::to_string(chunkCount), chunk);

		chunkCount++;
	}
}

//void ChunkManager::_unloadChunk(const glm::i32vec2& pos)
//{
//	if (_isLoaded(pos))
//	{
//		Chunk* ptr = _chunks[pos];
//
//		_chunks.extract(pos);
//		_tree.extract("chunk" + std::to_string(ptr->getID()));
//	}
//	else
//	{
//		std::cerr << "Tried to unload unloaded chunk" << std::endl;
//	}
//}

////////////////////////////////////////////////////////////////////////////////

void ChunkManager::_draw(RenderContext& context) const
{
	(void)context;
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

bool ChunkManager::_isLoaded(const glm::i32vec2& pos)
{
	return (_chunks.find(pos) != _chunks.end());
}
