#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <algorithm>
#include <cmath>
#include "Ant.h"
#include "Food.h"
#include "Pheromone.h"
#include "Constants.h"

using namespace std;

enum Scene {
	SurfaceWorld,
	UndergroundWorld
};

int main() {
	sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
	sf::RenderWindow window(desktopMode, "Ant Simulation", sf::Style::Default);
	window.setFramerateLimit(60);

	vector<Ant> ants;
	vector<Food> foodSources;
	vector<Pheromone> pheromones;

	// Nest setup
	sf::Vector2f nestPosition(desktopMode.width / 2, desktopMode.height / 2);
	sf::CircleShape nest(20);
	nest.setFillColor(sf::Color::White);
	nest.setPosition(nestPosition.x - 20, nestPosition.y - 20);

	// Create ants
	for (int i = 0; i < NUM_ANTS; ++i) {
		ants.emplace_back(nestPosition);
	}

	// Add food sources
	foodSources.emplace_back(sf::Vector2f(600, 400));
	foodSources.emplace_back(sf::Vector2f(200, 300));

	sf::Clock clock;

	// Create a view for controlling the visible area
	sf::View view(sf::FloatRect(0, 0, desktopMode.width, desktopMode.height));
	view.setCenter(nestPosition); // Start centered on the nest

	Scene currentScene = SurfaceWorld;
	while (window.isOpen()) {
		float dt = clock.restart().asSeconds();
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
				sf::Vector2i mousePos = sf::Mouse::getPosition(window);
				sf::Vector2f worldPos = window.mapPixelToCoords(mousePos, view); // Convert to world coordinates

				// Check if the click is on a pheromone
				for (auto& p : pheromones) {
					float dist = hypot(p.getPosition().x - worldPos.x, p.getPosition().y - worldPos.y);
					if (dist < 5.0f) { // Assuming pheromones are small dots
						currentScene = UndergroundWorld; // Transition to underground world
						break;
					}
					if (currentScene == SurfaceWorld) {
						// Surface world logic (existing code)
						// Update pheromones, ants, and draw the surface world
					}
					else if (currentScene == UndergroundWorld) {
						// Underground world logic
						window.clear(sf::Color(20, 20, 50)); // Darker background for underground

						// Example underground scene: glowing pheromone trails
						for (auto& p : pheromones) {
							sf::CircleShape glowingPheromone(10);
							glowingPheromone.setPosition(p.getPosition());
							glowingPheromone.setFillColor(sf::Color(0, 255, 255, static_cast<sf::Uint8>(p.strength)));
							window.draw(glowingPheromone);
						}

						// Add a return button or exit logic
						sf::Text returnText;
						sf::Font font;
						font.loadFromFile("C:\Windows\Fonts\simsunb.ttf"); // Ensure you have a font file
						returnText.setFont(font);
						returnText.setString("Press Esc to Return");
						returnText.setCharacterSize(24);
						returnText.setFillColor(sf::Color::White);
						returnText.setPosition(10, 10);
						window.draw(returnText);

						// Handle return to surface world
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
							currentScene = SurfaceWorld;
						}

						window.display();
						continue; // Skip the rest of the surface world logic
					}
				}
			}
		}

		// Handle view movement with keyboard (WASD or arrow keys)
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
			view.move(0, -200 * dt); // Move up
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			view.move(0, 200 * dt); // Move down
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			view.move(-200 * dt, 0); // Move left
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			view.move(200 * dt, 0); // Move right
		}

		// Update pheromones
		for (auto& p : pheromones) p.strength *= PHEROMONE_DECAY;
		pheromones.erase(remove_if(pheromones.begin(), pheromones.end(),
			[](const Pheromone& p) { return p.strength < 0.1f; }), pheromones.end());

		// Update ants
		for (auto& ant : ants) {
			ant.update(dt, pheromones, foodSources, nestPosition);

			if (!ant.hasFood) {
				for (auto& food : foodSources) {
					if (hypot(food.position.x - ant.position.x,
						food.position.y - ant.position.y) < 15.0f && food.quantity > 0) {
						ant.hasFood = true;
						food.quantity -= 1.0f;
					}
				}
			}
			else {
				if (hypot(nestPosition.x - ant.position.x,
					nestPosition.y - ant.position.y) < 30.0f) {
					ant.hasFood = false;
				}
			}
		}

		// Draw everything
		window.clear(sf::Color(50, 50, 50));

		// Set the view before drawing
		window.setView(view);

		for (auto& p : pheromones) {
			sf::CircleShape dot(3);
			dot.setPosition(p.getPosition());
			dot.setFillColor(p.hasFood ?
				sf::Color(0, 255, 0, static_cast<sf::Uint8>(p.strength)) :
				sf::Color(255, 255, 255, static_cast<sf::Uint8>(p.strength)));
			window.draw(dot);
		}

		for (auto& food : foodSources) {
			sf::CircleShape foodShape(10);
			foodShape.setFillColor(sf::Color::Yellow);
			foodShape.setPosition(food.position);
			window.draw(foodShape);
		}

		for (auto& ant : ants) {
			ant.draw(window);
		}

		window.draw(nest);
		window.display();
	}

	return 0;
}
