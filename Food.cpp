#pragma once
#include "Food.h"
#include "Constants.h"
#include <SFML/System.hpp>


Food::Food(sf::Vector2f pos) : position(pos), quantity(100.0f) {
    shape.setRadius(8);
    shape.setFillColor(FOOD_COLOR);
    shape.setPosition(position);
}