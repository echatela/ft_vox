#pragma once

#include "glm/ext/matrix_float4x4.hpp"
#include "scene/material.hpp"
#include <glm/ext/vector_int2_sized.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include <unordered_map>

#include "world/chunk.hpp"

class ChunkManager
{
	// TODO: maybe replace by <uint32_t, Chunk> for faster access
	std::unordered_map<glm::i32vec2, Chunk> _chunks;
	Material                                _material;

public:
	ChunkManager();

	void loadAround(const glm::vec3& pos);
	void draw(glm::mat4 matrix) const;

	void            setMaterial(const Material& mat);
	const Material& getMaterial() const;

private:
	void _loadChunk(const glm::i32vec2& pos);

	bool _isLoaded(const glm::i32vec2& pos);
};
