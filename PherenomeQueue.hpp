#pragma once
#include "Ant.h"
#include "Food.h"
#include <unordered_map>
#include <vector>
#include <utility>
#include <memory>
#include <algorithm>
#include <queue>
#include <set>
#include "Pheromone.h"
#include <array>


//Use to check if the ant already visited a pheromone.
class PherenomeQueue {
	
	
	// we are not going to be access pherenomes objects, we just care about the address, so i enforce that
	static constexpr uint8_t size = 30;
	std::array<void*,size> ps;
	uint8_t i = 0;
public:
	void push(Pheromone* p) {
		if (already_visited(p)) {
			return;
		}

		ps[i] = (void*)p;
		i =  (i + 1) % size;
	}
	
	bool already_visited(Pheromone* p) {
		for (int j = 0; j < size; ++j) {
			if (ps[j] == p) {
				return true;
			}
		}
		return false;
	}


};