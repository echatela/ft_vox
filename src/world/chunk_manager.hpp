#pragma once

#include "glm/ext/matrix_float4x4.hpp"
#include "scene/material.hpp"
#include <glm/ext/vector_int2_sized.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include <unordered_map>

#include "world/chunk.hpp"

class ChunkManager : public Node
{
	// TODO: maybe replace by <uint32_t, Chunk> for faster access
	std::unordered_map<glm::i32vec2, Chunk*> _chunks;
	Material                                _material;

	void _draw(RenderContext& context) const;
	
	void _loadAround(const glm::ivec2& pos);

	void _swapRange(const glm::ivec2& oldPos, const glm::ivec2& newPos);
	void _swapChunk(const glm::ivec2& oldPos, const glm::ivec2& newPos);

public:
	ChunkManager();

	void updateChunks(const glm::vec3& pos, bool preload = false);


	void            setMaterial(const Material& mat);
	const Material& getMaterial() const;

	unsigned int 	getSize() const;

private:
	void _loadChunk(const glm::i32vec2& pos);
	void _unloadChunk(const glm::i32vec2& pos);

	bool _isLoaded(const glm::i32vec2& pos);
};
