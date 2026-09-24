#include "world/chunk_manager/chunk_mesher.hpp"
#include "world/chunk_manager/chunk.hpp"
#include <cstdint>

// #define EDGES(yzx) \
//	((((yzx & 0xF) == 0) << kFaceLeft) +                                       \
//	 (((yzx & 0xF) == 0xF) << kFaceRight) +                                    \
//	 ((((yzx >> 4) & 0xF) == 0) << kFaceBack) +                                \
//	 ((((yzx >> 4) & 0xF) == 0xF) << kFaceFront) +                             \
//	 ((((yzx >> 8) & 0xFF) == 0) << kFaceDown) +                               \
//	 ((((yzx >> 8) & 0xFF) == 0xFF) << kFaceUp))

constexpr int kNeighbourOffset[6] = {
    +kStrideX, -kStrideX, // right, left
    +kStrideY, -kStrideY, // up, down
    +kStrideZ, -kStrideZ  // front, back
};

constexpr unsigned int kQuadIndices[kFaceCount] = {0, 1, 2, 0, 2, 3};

constexpr uint8_t edgeMask(unsigned int i)
{
	const unsigned int x = i & 0xF, y = (i >> 8) & 0xFF, z = (i >> 4) & 0xF;

	return ((x == kChunkWidth - 1) << kFaceRight | (x == 0) << kFaceLeft |
	        (y == kChunkHeight - 1) << kFaceUp | (y == 0) << kFaceDown |
	        (z == kChunkWidth - 1) << kFaceFront | (z == 0) << kFaceBack);
}

void ChunkMesher::build(Chunk& chunk, const Neighbours& neighbours)
{
	Vertex  v;
	uint8_t edges;

	for (unsigned int i = 0; i < kChunkSize; i++)
	{
		if (chunk._bitBlocks[i])
		{
			v.pos = i;
			v.id = chunk._blocks.at(i);
			edges = edgeMask(i);

			for (v.face = kFaceRight; v.face < kFaceCount; v.face++)
			{
				unsigned int baseIndex = chunk._vertices.size();
				unsigned int neighbourIndex = i + kNeighbourOffset[v.face];

				if ((edges >> v.face) & 1)
				{
					if (!neighbours[v.face])
					{
						for (v.corner = 0; v.corner < 4; v.corner++)
							chunk._vertices.push_back(v);
						for (int j = 0; j < 6; j++)
							chunk._indices.push_back(baseIndex +
							                         kQuadIndices[j]);
					}
				}
				else if (!chunk._bitBlocks[neighbourIndex])
				{
					for (v.corner = 0; v.corner < 4; v.corner++)
						chunk._vertices.push_back(v);
					for (int j = 0; j < 6; j++)
						chunk._indices.push_back(baseIndex + kQuadIndices[j]);
				}
			}
		}
	}
	chunk._vertices.shrink_to_fit();
	chunk._indices.shrink_to_fit();
	chunk._setupMesh();
}
