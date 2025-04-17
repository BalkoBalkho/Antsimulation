#include "World.h"

void World::populateChunk(Chunk& chunk) {
	for (int i = 0; i < 128; i++) {
		for (int j = 0; j < 256; j++) {
			chunk.chunkData[i][j] = WorldPixel::dirt;
		}
	}
}

