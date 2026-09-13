#pragma once

#include <glm/ext/vector_int2_sized.hpp>
#include <unordered_map>

#include "world/chunk.hpp"

class ChunkManager
{
	std::unordered_map<glm::i32vec2, Chunk> _chunks;

public:
	ChunkManager();
	~ChunkManager();

	void load(const glm::ivec3& playerPos);

	void draw() const;
};
