#pragma once
#include <SFML/System.hpp>
//Window and Simulation Settings
const int WIDTH = 800;  //Sets the width of the simulation window in pixels.
const int HEIGHT = 600;  //Sets the height of the simulation window in pixels.
const int NUM_ANTS = 50;  //Specifies the number of ants in the simulation.
//Pheromone Settings
const float PHEROMONE_DECAY = 0.97f;//Determines how quickly pheromones decay over time. A value closer to 1.0f means slower decay, while a lower value means faster decay.
const float PHEROMONE_STRENGTH = 255.0f;//Sets the initial strength of pheromones when they are deposited by ants.
//Ant Behavior and Appearance
const float ANT_SPEED = 40.0f;//Sets the movement speed of the ants in the simulation.
const sf::Color ANT_BODY_COLOR(139, 69, 19);//Defines the color of the ant's body (brown).
const sf::Color ANT_HEAD_COLOR(50, 20, 10);//Defines the color of the ant's head (darker brown).
const sf::Color ANT_LEG_COLOR(100, 65, 30);//Defines the color of the ant's legs (lighter brown).
//Nest and Food Appearance
const sf::Color NEST_COLOR(178, 34, 34);//Defines the color of the nest (firebrick red).
const sf::Color FOOD_COLOR(34, 139, 34);//Defines the color of food sources (forest green).
const sf::Color BACKGROUND_COLOR(240, 240, 220);//Sets the background color of the simulation (light beige).
//Pheromone Detection Settings
const float SENSOR_ANGLE = 0.3f;  //Specifies the angle (in radians) used by ants to detect pheromones in their surroundings.
const float SENSOR_DISTANCE = 30.0f;//Sets the distance within which ants can detect pheromones.