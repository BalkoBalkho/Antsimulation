#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Pheromone.h"
#include "Constants.h"
#include "Food.h" 

class Colony {

};


class Ant {
    private:
        float speed;  //Determines how fast the ant moves in the simulation.
        float legAngleOffset;  // Controls the offset for animating the ant's leg movement.
        float animationTimer;  // Tracks time for animating the ant's legs or other body parts.
        float bodyRotation;  // Stores the current rotation angle of the ant's body for directional movement.

        sf::ConvexShape body;//Represents the main body of the ant. A convex shape allows for a custom polygonal shape to visually depict the ant's body.
        sf::CircleShape head;// Represents the head of the ant as a circle.
        sf::CircleShape foodIndicator;//A small circle used to indicate whether the ant is carrying food (e.g., drawn when hasFood is true).
        std::vector<sf::RectangleShape> legs;// A collection of rectangle shapes representing the ant's legs, which can be animated for movement.

        void updateBodyParts();
    public:
        sf::Vector2f position; // Tracks the ant's location in the simulation.
        sf::Vector2f direction; //Tracks the direction the ant is moving.
        bool hasFood;  //Tracks whether the ant is carrying food.
        bool isSelected;  //Tracks whether the ant is selected by the user for interaction or tracking.

        Ant(sf::Vector2f pos);  //declaration of the constructor

        void update(float dt, std::vector<Pheromone>& pheromones, const std::vector<Food>& foods, const sf::Vector2f& nestPos);//handles ants movement, interactions with food, pheromones, and the nest.
        void draw(sf::RenderWindow& window);   //renders ant on the screen
        void rotate(float angle);    //changes ants direction by a specified angle

};
