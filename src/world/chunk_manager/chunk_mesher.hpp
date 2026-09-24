#pragma once

#include "world/chunk_manager/chunk.hpp"

#include <array>

using Neighbours = std::array<Chunk*, kFaceCount>;

class ChunkMesher
{
public:
	static void build(Chunk& chunk, const Neighbours& neighbours);
};
