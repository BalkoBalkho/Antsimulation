#include "Pheromone.h"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

// Pheromone::Pheromone(sf::Vector2f pos, float str, bool food)
//     : position(pos), strength(str), hasFood(food) {}

sf::Vector2f Pheromone::getPosition() const {
    return position;
}

float Pheromone::getStrength() const {
    return strength;
}

pherotype Pheromone::getType() const
{
    return type;
}

bool Pheromone::isFood() const {
    return hasFood;
}
