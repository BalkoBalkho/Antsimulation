#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Ant.h"
#include "Food.h"
#include "Pheromone.h"
#include "Constants.h"
#include "World.h"
#include "Simulation.h"
#include "MindControl.hpp"

using namespace std;

sf::Vector2f getRandomPosition(const sf::FloatRect& bounds)
{
    float x = bounds.left + static_cast<float>(std::rand()) / RAND_MAX * bounds.width;
    float y = bounds.top + static_cast<float>(std::rand()) / RAND_MAX * bounds.height;
    return sf::Vector2f(x, y);
}

int main()
{
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(desktopMode, "Ant Simulation", sf::Style::Default);
    window.setFramerateLimit(60);

    
    siml.foodSources.push_back(make_shared<Food>((sf::Vector2f(600, 400)))); // Example food
    siml.foodSources.push_back(make_shared<Food>((sf::Vector2f(800, 500))));
    


    //vector<sf::RectangleShape> obstacles;
    sf::FloatRect worldBounds(0, 0, desktopMode.width * 1, desktopMode.height * 1);

    sf::View miniMapView(sf::FloatRect(0, 0, worldBounds.width, worldBounds.height));
    miniMapView.setViewport(sf::FloatRect(0.8f, 0.8f, 0.2f, 0.2f)); // Bottom-right corner

    float zoomLevel = 1.0f;
    const float zoomSpeed = 0.1f;
    const float minZoom = 0.5f;
    const float maxZoom = 2.5f;

    // draw mini map
    window.setView(miniMapView);

    // Draw ants
    for (auto colony : siml.colonies)
    for (auto& ant : colony->ants)
    {
        sf::CircleShape antDot(2); // Smaller dots for mini-map
        antDot.setFillColor(sf::Color::Red);
        antDot.setPosition(ant->getPosition());
        window.draw(antDot);
    }

    // Draw food
    for (auto& food : siml.foodSources)
    {
        sf::CircleShape foodDot(3);
        foodDot.setFillColor(sf::Color::Green);
        foodDot.setPosition(food->getPosition());
        window.draw(foodDot);
    }

    // Draw obstacles
    // const int numObstacles = 10; // Number of obstacles
    // for (int i = 0; i < numObstacles; ++i)
    // {
    //     sf::RectangleShape obstacle(sf::Vector2f(100, 50)); // Example size
    //     obstacle.setFillColor(sf::Color(100, 100, 100));    // Gray color
    //     obstacle.setPosition(getRandomPosition(worldBounds));
    //     obstacles.push_back(obstacle);
    // }

    // Nest setup
    sf::Vector2f nestPosition(desktopMode.width / 2, desktopMode.height / 2);
    std::vector<sf::CircleShape> nestLayers;
    const int nestLayersCount = 5;
    const float baseRadius = 40.0f;

    for (int i = 0; i < nestLayersCount; ++i)
    {
        sf::CircleShape layer(baseRadius - i * 6);              // Decreasing radius
        sf::Uint8 alpha = static_cast<sf::Uint8>(200 - i * 30); // More transparent outward
        layer.setFillColor(sf::Color(139, 69, 19, alpha));      // Brownish color with transparency
        layer.setOrigin(layer.getRadius(), layer.getRadius());
        layer.setPosition(nestPosition);
        nestLayers.push_back(layer);
    }
    

    // Seed the random number generator
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // Place food at random positions
    const int numFoodPieces = 50; // Number of food pieces to place
    for (int i = 0; i < numFoodPieces; ++i)
    {
        sf::Vector2f randomPosition = getRandomPosition(worldBounds);
        siml.foodSources.push_back(make_shared<Food>(randomPosition)); // Add food to the world

    }

    auto colony1 = std::make_shared<Colony>(NUM_ANTS, sf::Color(139, 69, 19), nestPosition );
    colony1->ants.resize(NUM_ANTS);
    siml.colonies.push_back(colony1);




    // // Create ants
    // for (int i = 0; i < NUM_ANTS; ++i)
    // {
    //     // ants.emplace_back(nestPosition, sf::Color(139, 69, 19)); // Brown color
    // }

    // Create another species of ants
    sf::Vector2f otherNestPosition(desktopMode.width / 2, desktopMode.height * 9 / 4); // Moved lower
    std::vector<sf::CircleShape> otherNestLayers;
    for (int i = 0; i < nestLayersCount; ++i)
    {
        sf::CircleShape layer(baseRadius - i * 6);
        sf::Uint8 alpha = static_cast<sf::Uint8>(200 - i * 30);
        layer.setFillColor(sf::Color(0, 0, 0, alpha)); // Dodger blue with transparency
        layer.setOrigin(layer.getRadius(), layer.getRadius());
        layer.setPosition(otherNestPosition);
        otherNestLayers.push_back(layer);
    }

    auto colony2 = std::make_shared<Colony>(NUM_ANTS, sf::Color::Black, otherNestPosition);
    //colony2->ants.resize(NUM_ANTS);
    siml.colonies.push_back(colony2);
    // constexpr int  NUM_OTHER_ANTS = 30;
    // for (int i = 0; i < NUM_OTHER_ANTS; ++i)
    // {
    //     otherSpeciesAnts.emplace_back(otherNestPosition, sf::Color::Black); // Black color
    // }

    // Load food with textures
    siml.foodSources.emplace_back(make_shared<Food>(sf::Vector2f(600, 400))); // Example food
    siml.foodSources.emplace_back(make_shared<Food>(sf::Vector2f(800, 500)));
    sf::Clock clock;
    float fast_foward = 1.0f;

    // Create a view for controlling the visible area
    sf::View view(sf::FloatRect(0, 0, desktopMode.width, desktopMode.height));
    view.setCenter(nestPosition);

    //World world;
    siml.world;
    //world.generateWorld(); // Initialize the world with noise generation

    sf::SoundBuffer eatBuffer;
    siml.eatSound;
    
    if (!eatBuffer.loadFromFile("./eat_crunch.ogg"))
    {
        std::cerr << "Failed to load eating sound!" << std::endl;
    }
    siml.eatSound.setBuffer(eatBuffer);
    siml.eatSound.setVolume(100);

    while (window.isOpen())
    {
        window.setView(miniMapView);

        float dt = clock.restart().asSeconds() * fast_foward;
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            // --- Keyboard zoom controls ---
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Add) || sf::Keyboard::isKeyPressed(sf::Keyboard::Equal))
            {
                zoomLevel = std::max(minZoom, zoomLevel - zoomSpeed);
                view.setSize(desktopMode.width * zoomLevel, desktopMode.height * zoomLevel);
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract) || sf::Keyboard::isKeyPressed(sf::Keyboard::Dash))
            {
                zoomLevel = std::min(maxZoom, zoomLevel + zoomSpeed);
                view.setSize(desktopMode.width * zoomLevel, desktopMode.height * zoomLevel);
            }
        }

        // Update other species of ants
        for (auto& colony : siml.colonies)
        {
            colony->update(dt);
        }

        window.setView(view); // After the pollEvent loop

        // Handle view movement with keyboard (WASD or arrow keys)
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            view.move(0, -1000 * dt);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            view.move(0, 1000 * dt);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            view.move(-1000 * dt, 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            view.move(1000 * dt, 0);
        }


        if (sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
        {
            if (fast_foward == 1.0f)
                fast_foward = 2.5f; // it cant be too high because the collision check becomes innacurate unfortuantly
            // only way to fix that would be to use a raycast 
			else
				fast_foward = 1.0f;
        }

        
         siml.soundplayed = false;
        
        // Draw everything
        window.clear(sf::Color(50, 50, 50));

        // Draw the world terrain
        
        window.setView(view);
        window.draw(siml.world.vertices);
        
        


        // Draw animated pheromones
        float time = clock.getElapsedTime().asSeconds(); // Use elapsed time for animation
        for (auto& c : siml.colonies)
        for (auto& p : c->pheromones)
        {
            float pulse = 1.0f + 0.2f * std::sin(time * 3.0f); // Pulsing effect
            float size = std::max(1.0f, p->strength / 50.0f) * pulse * 3;

            sf::CircleShape dot(size);
            dot.setPosition(p->getPosition() - sf::Vector2f(size / 2, size / 2)); // Center the dot
            sf::Uint8 alpha =  static_cast<sf::Uint8>(std::min(255.0f, p->strength *150));
            dot.setFillColor(p->hasFood ? sf::Color(0, 255, 0, alpha) : sf::Color(255, 255, 255, alpha));
            window.draw(dot);
        }

        for (auto& food : siml.foodSources)
        {
            sf::CircleShape foodShape(10);
            foodShape.setFillColor(sf::Color::Yellow);
            foodShape.setPosition(food->getPosition());
            window.draw(foodShape);
        }

        // Draw ants
        for (auto& c : siml.colonies)
        for (auto& ant : c->ants)
        {
            ant->draw(window);
        }

        // Draw nests
        for (auto& layer : nestLayers)
        {
            window.draw(layer);
        }
        for (auto& layer : otherNestLayers)
        {
            window.draw(layer);
        }
        // for (auto& obstacle : obstacles)
        // {
        //     window.draw(obstacle);
        // }

        window.setView(view);
        window.setView(miniMapView);

        window.draw(siml.world.vertices);
        window.display();
    }

    return 0;
}