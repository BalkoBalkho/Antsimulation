#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>


class Food {
public:
    sf::Vector2f position;
    float quantity;
    sf::CircleShape shape;

    Food(sf::Vector2f pos);
};