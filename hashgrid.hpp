#pragma once
#include <unordered_map>
#include <vector>
#include <utility>
#include <memory>
#include <algorithm>

template <typename T>
class HashGrid {
    static const int CELL_SIZE = 10;
    std::unordered_map<std::pair<int, int>, std::vector<std::weak_ptr<T>>> grid;

public:
    // Add an object to the grid (using a shared_ptr)
       void add(const std::shared_ptr<T>& obj) {
        auto pos = obj->position;
        int cellX = static_cast<int>(pos.x) / CELL_SIZE;
        int cellY = static_cast<int>(pos.y) / CELL_SIZE;
        grid[{cellX, cellY}].push_back(obj); // Convert shared_ptr to weak_ptr
    }

    // Get nearby objects (returns shared_ptrs)
    std::vector<std::shared_ptr<T>> getNearby(sf::Vector2f pos) {
        int cellX = static_cast<int>(pos.x) / CELL_SIZE;
        int cellY = static_cast<int>(pos.y) / CELL_SIZE;
        std::vector<std::shared_ptr<T>> nearbyObjects;

        for (int x = cellX - 1; x <= cellX + 1; ++x) {
            for (int y = cellY - 1; y <= cellY + 1; ++y) {
                auto it = grid.find({ x, y });
                if (it != grid.end()) {
                    for (const auto& weakObj : it->second) {
                        if (auto obj = weakObj.lock()) {
                            nearbyObjects.push_back(obj);
                        }
                    }
                }
            }
        }

        return nearbyObjects;
    }



    // Update the grid (remove expired entries)
    void update() {
        for (auto& [cell, objects] : grid) {
            objects.erase(
                std::remove_if(objects.begin(), objects.end(),
                    [](const std::weak_ptr<T>& wp) { return wp.expired(); }),
                objects.end()
            );

            if (objects.empty()) {
                continue;
            }

            std::vector<std::weak_ptr<T>> toRemove;

            for (auto& wp : objects) {
                auto obj = wp.lock();
                if (!obj) continue;

                // Only try to move if the object supports movement
                if constexpr (T::is_movable) {
                    sf::Vector2f oldPos = obj->position;
                    obj->move(0.016f); // Or however you pass delta time
                    sf::Vector2f newPos = obj->position;

                    int oldCellX = static_cast<int>(oldPos.x) / CELL_SIZE;
                    int oldCellY = static_cast<int>(oldPos.y) / CELL_SIZE;
                    int newCellX = static_cast<int>(newPos.x) / CELL_SIZE;
                    int newCellY = static_cast<int>(newPos.y) / CELL_SIZE;

                    if (newCellX != oldCellX || newCellY != oldCellY) {
                        grid[{newCellX, newCellY}].push_back(wp);
                        toRemove.push_back(wp);
                    }
                }
            }

            for (const auto& wp : toRemove) {
                objects.erase(std::remove(objects.begin(), objects.end(), wp), objects.end());
            }

            if (objects.empty()) {
                grid.erase(cell);
            }
        }
    }

    void add(const std::list<T> list&) {
        for (auto& obj : list) {
            add((obj));
        }
    }

};