// Pheromone.h
#pragma once
#include <SFML/System.hpp>


enum class pherotype { food, danger, war, colony,	//paths
	
	cleaning=10, patrol, food_finding, transport, };//jobs

class Pheromone {  
	pherotype type;
   public:  
   
   Pheromone(sf::Vector2f pos, pherotype type, float strength=1.0f, bool is_request=false,bool food=false) : position(pos), type(type), strength(strength), hasFood(food), is_request(is_request) {};  
   sf::Vector2f getPosition() const;  
   float getStrength() const;  
   pherotype getType() const;
   
   bool isFood() const;
   bool is_path();
   bool is_job();
   bool is_request;
   
   sf::Vector2f position;  
   float strength;  
   bool hasFood;  
};

