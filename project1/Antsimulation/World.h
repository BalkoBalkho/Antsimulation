#pragma once

#include <array>
#include <vector>


class World {

public:
	enum class WorldPixel :uint8_t {
		dirt = 0,
		grass = 1,
		rock = 2,
		water = 3,
		food = 4,
		ant = 5, // ants have different body parts
		tree = 6,
		pherenome = 7,

	};


private: 
	#define chunkHeight 128
	const int chunkWidth = 256;

	struct Chunk {


		std::array<std::array<WorldPixel, chunkHeight>, 256> chunkData;
		//Chunk* next;
		//Chunk* prev;

		std::array<WorldPixel,chunkHeight>& getY(int y) {
			return chunkData[y];
		}
	};
	// Why a dynamic array instead of a linked list?
	// Because we will be reading and accessing indexes more than we will be adding or removing chunks
	/// O(1) < O(n)
	std::vector<Chunk> chunklist; 
public:



	World() {
		chunklist.emplace_back();
		populateChunk(chunklist[0]);
	};
	

	std::array<WorldPixel, chunkHeight>& operator[](int x) {
		int chunkIndex = x / chunkHeight;
		int y = x % 128;
	
		return chunklist[chunkIndex].getY(y);
	}
	void populateChunk(Chunk& chunk);
	

	

};