#pragma once

#include <array>
#include <vector>
#include "Pherenome.h"


struct cords { float x; float y; };


class World {

public:
	// publically accesible enum
	struct WorldPixel  {

	
	// 64 + 8 bits per cell
	// 256 * 256 * (64*8) -> 33554432 bits -> 4.194304 mb per "chunk". not bad not good
	const enum class type :uint8_t {
		dirt = 0,
		grass = 1,
		rock = 2,
		water = 3,
		food = 4,
		ant = 5, // ants have different body parts
		tree = 6,
		pherenome = 7,

	};
	PherenomeMarker marker;

	};


private: 
	#define chunkHeight 256
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
		if (chunkIndex >= chunklist.size()) {
			chunklist.resize(chunkIndex + 1);
			populateChunk(chunklist[chunkIndex]);
		}

		return chunklist[chunkIndex].getY(y);
	}
	void populateChunk(Chunk& chunk);
	

	

};