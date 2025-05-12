#pragma once
#include "Ant.h"
#include "Food.h"
#include "Pheromone.h"
#include "hashgrid.hpp"
#include <list>
#include "World.h"
#include <SFML/Audio.hpp>


//globals
 struct simulation {
    std::list<std::shared_ptr<Colony>> colonies;
    std::list<std::shared_ptr<Food>> foodSources;
    Ant* controlled_ant = nullptr;
    bool is_ant_controlled = false;
    float dt = 16.0f;
    uint64_t seed = 0;
    HashGrid<Ant> antGrid;
    HashGrid<Food> foodGrid;
    World* world;

    sf::Sound eatSound;
    bool soundplayed=false;


    
    // Optional: Add a function to add new ants/food
    // void addAnt(const std::shared_ptr<Ant>& ant) {
    //     // Assuming you have a way to get a shared_ptr<Ant>
    //     antGrid.add(ant);

    //     // You might also add to a global list or something else
    // }

    // void addFood(std::shared_ptr<Food> food) {
    //     foodGrid.add(food);
    //     // Also add to foodSources if needed
    // }
};

extern struct simulation siml;

inline float get_distance(sf::Vector2f a, sf::Vector2f b) {
    return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}