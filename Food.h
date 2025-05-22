#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>


class Food {
    sf::Vector2f position;
public:
    float quantity;
    sf::CircleShape shape;

    sf::Vector2f getPosition() { return position; }
    Food(sf::Vector2f pos);
};