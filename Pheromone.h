// Pheromone.h
#pragma once
#include <SFML/System.hpp>


enum class pherotype { food, danger, war, colony,	//paths
	
	cleaning=10, patrol, food_finding, transport, };//jobs

class Pheromone {  
	pherotype type = pherotype::colony;
   public:  
	   Pheromone() = default;
   Pheromone(sf::Vector2f pos, pherotype type, float strength=1.0f, bool is_request=false,bool food=false) : position(pos), type(type), strength(strength), hasFood(food), is_request(is_request) {};  
   sf::Vector2f getPosition() const;  
   float getStrength() const;  
   pherotype getType() const;
   
   bool isFood() const;
   //bool is_path() {return };
   bool is_job() {
	   if (int(this->type) >= 10) return true;
	   return false;
   };
   bool is_request =false;
   
   sf::Vector2f position;  
   float strength;  
   bool hasFood;  
};

