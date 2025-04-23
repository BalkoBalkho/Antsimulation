#include "UndergroundScene.h"

UndergroundScene::UndergroundScene(sf::RenderWindow* window) : window(window) {
    // Load font for text
    font.loadFromFile("path/to/font.ttf");
    infoText.setFont(font);
    infoText.setString("Welcome to the Underground World!");
    infoText.setCharacterSize(24);
    infoText.setFillColor(sf::Color::White);
    infoText.setPosition(50, 50);
}

void UndergroundScene::loadScene() {
    // Initialize the underground world (e.g., tunnels, glowing trails)
}

void UndergroundScene::update(float dt) {
    // Handle animations or interactions in the underground world
}

void UndergroundScene::draw() {
    // Draw the underground world
    window->clear(sf::Color::Black); // Set background to black
    window->draw(infoText); // Draw the welcome text
    // Add more drawing logic here (e.g., tunnels, glowing trails)
    window->display();
}