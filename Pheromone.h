// Pheromone.h
#pragma once
#include <SFML/System.hpp>

class Pheromone {
public:
    Pheromone(sf::Vector2f pos, float str, bool food);
    sf::Vector2f getPosition() const;
    float getStrength() const;
    bool isFood() const;
    sf::Vector2f position;
    float strength;
    bool hasFood;
};

