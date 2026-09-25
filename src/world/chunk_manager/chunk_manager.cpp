#include "chunk_manager.hpp"
#include "glm/ext/vector_int2.hpp"
#include "glm/ext/vector_int2_sized.hpp"
#include "loader/resource_manager.hpp"
#include "render/texture_2d_array.hpp"
#include "scene/material.hpp"
#include "world/chunk_manager/chunk.hpp"
#include "world/chunk_manager/chunk_mesher.hpp"

constexpr int kLoadDistance = 10;
constexpr int kLoadRange = kLoadDistance * 2 + 1;
constexpr int kLoadCount = kLoadRange * kLoadRange;
// constexpr int kPreloadDistance = 11;
// constexpr int kPreloadRange = kPreloadDistance * 2 + 1;
// constexpr int kPreloadCount = kPreloadRange * kPreloadRange;

ChunkManager::ChunkManager(const glm::vec3& worldPos)
{
	_material.shader =
	    ResourceManager::instance().get<Shader>(ResourceId::SHADER_CHUNK);
	_material.texture = ResourceManager::instance().get<Texture2DArray>(
	    ResourceId::TEXTURE_BLOCKS);

	_chunkMemory = new Chunk[kLoadCount];
	_chunks.reserve(kLoadCount);
	_loadAround(worldPos);
}

ChunkManager::~ChunkManager()
{
	delete[] _chunkMemory;

	_tree.clear();
}

////////////////////////////////////////////////////////////////////////////////

void ChunkManager::updateChunks(const glm::vec3& worldPos)
{
	static glm::i32vec2 lastPos = {0, 0};
	const glm::i32vec2  chunkPos = {std::floor(worldPos.x / kChunkWidth),
	                                std::floor(worldPos.z / kChunkWidth)};

	if (chunkPos != lastPos)
	{
		_swapRange(lastPos, chunkPos);
	}

	lastPos = chunkPos;
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
void ChunkManager::_swapChunk(const glm::i32vec2& oldPos,
                              const glm::i32vec2& newPos)
{
	Chunk* chunk = _chunks[oldPos];

	// reuse allocated chunk
	chunk->clear();
	chunk->generate(newPos, 0);
	ChunkMesher::build(*chunk, _neighbours(newPos));

	// insert new pair & remove old
	_chunks.erase(oldPos);
	_chunks[newPos] = chunk;
}

void ChunkManager::_loadAround(const glm::vec3& worldPos)
{
	const glm::i32vec2 chunkPos = {std::floor(worldPos.x / kChunkWidth),
	                               std::floor(worldPos.z / kChunkWidth)};
	const glm::i32vec2 startPos = chunkPos - kLoadDistance;
	const glm::i32vec2 endPos = chunkPos + kLoadDistance;

	glm::i32vec2 v;
	for (v.y = startPos.y; v.y <= endPos.y; v.y++)
	{
		for (v.x = startPos.x; v.x <= endPos.x; v.x++)
		{
			_loadChunk(v);
		}
	}
}

void ChunkManager::_loadChunk(const glm::i32vec2& pos)
{
	static unsigned int chunkCount = 0;

	if (!_isLoaded(pos))
	{
		Chunk* chunk = &(_chunkMemory[chunkCount]);

		chunk->setMaterial(_material);
		chunk->setID(chunkCount);

		chunk->generate(pos, 0);
		ChunkMesher::build(*chunk, _neighbours(pos));
		_chunks.insert({pos, chunk});

		append("chunk" + std::to_string(chunkCount), chunk);

		chunkCount++;
	}
}

namespace
{
constexpr Face kFaceSide[4] = {kFaceRight, kFaceLeft, kFaceDown, kFaceBack};
constexpr glm::i32vec2 kSideOffset[4] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
} // namespace

// NOTE: for now up and down are nullptr but if we implement the 16 16 16 chunk
// it will not be the case anymore
Neighbours ChunkManager::_neighbours(const glm::i32vec2& pos)
{
	Neighbours n{};

	for (int i = 0; i < 4; i++)
	{
		auto it = _chunks.find(pos + kSideOffset[i]);
		if (it != _chunks.end())
			n[kFaceSide[i]] = it->second;
	}
	return n;
}

// void ChunkManager::_unloadChunk(const glm::i32vec2& pos)
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
// }

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
