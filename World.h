#pragma once
#include "FastNoiseLite.h"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
class World {


//std::array<std::array<

//sf::VertexArray vertices(sf::Quads, GRID_WIDTH* GRID_HEIGHT * 4);
const size_t GRID_WIDTH = 2048;
const size_t GRID_HEIGHT = 2048;
const size_t TILE_SIZE = 16; // Each tile is 32x32 pixels


sf::Color getColorFromNoise(float x, float y) {
	// Get noise value
	float value = noise.GetNoise(x, y);
	// Map the noise value to a color (example: grayscale)
	
    sf::Color color;
    if (value < -0.2f) {
        color = sf::Color(0, 0, 139); // Deep water
    }
    else if (value < 0.0f) {
        color = sf::Color(65, 105, 225); // Shallow water
    }
    else if (value < 0.2f) {
        color = sf::Color(210, 180, 140); // Sand
    }
    else if (value < 0.5f) {
        color = sf::Color(34, 139, 34); // Grass
    }
    else if (value < 0.8f) {
        color = sf::Color(139, 137, 137); // Mountain
    }
    else {
        color = sf::Color(255, 250, 250); // Snow
    }

	return color;
}

public:
sf::VertexArray vertices;
FastNoiseLite noise;



World() : vertices(sf::Quads, GRID_WIDTH * GRID_HEIGHT * 6) {
	// Initialize the vertex array with the correct size and type
    vertices.setPrimitiveType(sf::Triangles);
    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
}

void j() {
    


    for (int x = 0; x < GRID_WIDTH; x++) {
        for (int y = 0; y < GRID_HEIGHT; y++) {
            
            // Get noise value and color as before
            sf::Color color = getColorFromNoise(x, y);
            // Calculate base index (6 vertices per tile)
            int baseIndex = (x + y * GRID_WIDTH) * 6;

            // Define positions (same as quad corners)
            sf::Vector2f topLeft(x * TILE_SIZE, y * TILE_SIZE);
            sf::Vector2f topRight((x + 1) * TILE_SIZE, y * TILE_SIZE);
            sf::Vector2f bottomRight((x + 1) * TILE_SIZE, (y + 1) * TILE_SIZE);
            sf::Vector2f bottomLeft(x * TILE_SIZE, (y + 1) * TILE_SIZE);

            // First triangle (top-left, top-right, bottom-right)
            vertices[baseIndex + 0].position = topLeft;
            vertices[baseIndex + 1].position = topRight;
            vertices[baseIndex + 2].position = bottomRight;

            // Second triangle (top-left, bottom-right, bottom-left)
            vertices[baseIndex + 3].position = topLeft;
            vertices[baseIndex + 4].position = bottomRight;
            vertices[baseIndex + 5].position = bottomLeft;

            // Set colors for all 6 vertices
            for (int i = 0; i < 6; i++) {
                vertices[baseIndex + i].color = color;
                
            }
        }
    }


}
};