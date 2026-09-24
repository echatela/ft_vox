#pragma once

#include "scene/material.hpp"
#include "world/chunk_manager/chunk_mesher.hpp"
#include <glm/ext/vector_int2_sized.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include <unordered_map>

#include "world/chunk_manager/chunk.hpp"

class ChunkManager : public Node
{
	Chunk*                                   _chunkMemory = nullptr;
	std::unordered_map<glm::i32vec2, Chunk*> _chunks;
	Material                                 _material;

public:
	ChunkManager(const glm::vec3& worldPos = {0,0,0});
	~ChunkManager();

	void updateChunks(const glm::vec3& pos);

	void            setMaterial(const Material& mat);
	const Material& getMaterial() const;

	unsigned int getSize() const;

private:
	void _draw(RenderContext& context) const;

	void _loadAround(const glm::vec3& worldPos);

	void _swapRange(const glm::i32vec2& oldPos, const glm::i32vec2& newPos);
	void _swapChunk(const glm::i32vec2& oldPos, const glm::i32vec2& newPos);

	Neighbours _neighbours(const glm::i32vec2& pos);

	void _loadChunk(const glm::i32vec2& pos);
	void _unloadChunk(const glm::i32vec2& pos);

	bool _isLoaded(const glm::i32vec2& pos);
};
