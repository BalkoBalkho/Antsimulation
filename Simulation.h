#pragma once  
#include "Ant.h"  
#include "Food.h"  
#include "Pheromone.h"  
#include "hashgrid.hpp"  
#include <list>
#include "World.h"
struct simulation {  
   
   std::list<Colony> colonies;  
   std::list<Food> foodSources;  
   Ant* controlled_ant = nullptr;  
   bool is_ant_controlled = false;  
   float dt = 16.0f;  
   uint64_t seed = 0;  
   HashGrid<Ant> antGrid;  
   HashGrid<Food> foodGrid;  
   World* world;
   // Look at Colony.pherenomeGrid  
   // HashGrid<Pheromone*> pheromoneGrid; maybe in future ants could sniff enemy pheromones, idk what the exact research is but its possible for them to ginore it  
};  
simulation siml;

float get_distance(sf::Vector2f a, sf::Vector2f b) {
	sf::Vector2f dirToTarget = b - a;
   float distance = std::sqrt(dirToTarget.x * dirToTarget.x + dirToTarget.y * dirToTarget.y);
}