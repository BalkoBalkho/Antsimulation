#pragma once
#include <unordered_map>
#include <vector>
#include <utility>
#include <memory>
#include <algorithm>
#include <map>


namespace std {
    template <>
    struct hash<std::pair<int, int>> {
        size_t operator()(const std::pair<int, int>& p) const {
            return hash<int>()(p.first) ^ (hash<int>()(p.second) << 1);
        }
    };
}

template <typename T>
class HashGrid {

	static const int hashSize = 1000; // Size of the hash table
	std::array<std::vector<std::weak_ptr<T>>,hashSize> grid;
    std::hash<std::pair<int, int>> hasher;


    static const int CELL_SIZE = 10;
    //std::unordered_map<std::pair<int, int>, std::vector<std::weak_ptr<T>>> grid;

public:
    HashGrid() = default;

    // Add an object to the grid (using a shared_ptr
    // Add an object to the grid (using a shared_ptr)
       void add(const std::shared_ptr<T>& obj) {
        auto pos = obj->position;
        int cellX = static_cast<int>(pos.x) / CELL_SIZE;
        int cellY = static_cast<int>(pos.y) / CELL_SIZE;
		std::pair<int, int> cell = { cellX, cellY };
		auto hashv = hasher(cell);
		grid.at(hashv % hashSize).push_back(obj);
		//grid[cell].push_back(obj);
    }

    // Get nearby objects (returns shared_ptrs)
    std::vector<std::shared_ptr<T>> getNearby(sf::Vector2f pos) {
		std::vector<std::shared_ptr<T>> nearbyObjects;
		int cellX = static_cast<int>(pos.x) / CELL_SIZE;
		int cellY = static_cast<int>(pos.y) / CELL_SIZE;
		// Check the current cell and adjacent cells
		for (int dx = -1; dx <= 1; ++dx) {
			for (int dy = -1; dy <= 1; ++dy) {
				std::pair<int, int> cell = { cellX + dx, cellY + dy };
				auto hashv = hasher(cell);
				auto& objects = grid.at(hashv % hashSize);
				for (const std::weak_ptr<T> obj : objects) {
					if (!obj.expired()) {
						nearbyObjects.push_back(obj.lock());
					}
				}
			}
		}
		return nearbyObjects;
    }



    // Update the grid (remove expired entries)
    void update() {
		for (auto& cell : grid) {
			cell.remove_if([](const std::weak_ptr<T>& obj) {
                return obj == nullptr || obj.isExpired();
				});
		}
    }

    void add(const std::list<std::shared_ptr<T>>& list) {
        for (auto& obj : list) {
            add((obj));
        }
    }

};