#pragma once
#include "FastNoiseLite.h"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <array>
class World {


//std::array<std::array<

//sf::VertexArray vertices(sf::Quads, GRID_WIDTH* GRID_HEIGHT * 4);
   // funny how this was 2048 x2048 and it lagged my igpu so bad.
    // it was hard to diagnose since asmas unified architcure lets her run it in smooth
    // you could either run this with your dgpu for smooth fps or reduce the grid size
static constexpr size_t GRID_WIDTH{ 512 };
static constexpr size_t GRID_HEIGHT = 512;
static constexpr size_t TILE_SIZE = 16; // Each tile is 32x32 pixels


enum class material_type : uint8_t {
    DEEP_WATER,
    SHALLOW_WATER,
    SAND,
    GRASS,
    MOUNTAIN,
    SNOW,
    DIRT_WALL,
    STONE_WALL
};
struct Material {

	sf::Color color;
    bool isThisTraversable;
    bool isthisMineAble; // or diggable
	//bool isThisWater; certain ant species can swim (notably fire ants)

};

const std::map<material_type, Material> materialMap = {
    { material_type::DEEP_WATER, { sf::Color(0, 0, 139), false, false } },
    { material_type::SHALLOW_WATER, { sf::Color(65, 105, 225), true, false } },
    { material_type::SAND, { sf::Color(210, 180, 140), true, true } },
    { material_type::GRASS, { sf::Color(34, 139, 34), true, false } },
    { material_type::MOUNTAIN, { sf::Color(139, 137, 137), false, true } },
    { material_type::SNOW, { sf::Color(255, 250, 250), true, false } },
    { material_type::DIRT_WALL, { sf::Color(139, 69, 19), false, true } },
    { material_type::STONE_WALL, { sf::Color(128, 128, 128), false, false } } };


material_type getColorFromNoise(float x, float y) {
	// Get noise value
	float value = noise.GetNoise(x, y);
	
    if (value < -0.2f) {
        return material_type::GRASS; // Deep water
    }
    else if (value < 0.0f) {
        return material_type::SAND; // Shallow water
    }
    else if (value < 0.2f) {
        return material_type::SAND; // Sand
    }
    else if (value < 0.5f) {
        return material_type::GRASS; // Grass
    }
    else if (value < 0.8f) {
        return material_type::MOUNTAIN; // Mountain
    }
    else {
        return material_type::SHALLOW_WATER; // Snow
    }
}



std::array<std::vector<material_type>, GRID_HEIGHT> grid = {}; // 2D array to store material types. Sisters query with their antanenae (and eyes)
public:
sf::VertexArray vertices;
FastNoiseLite noise;

material_type getMaterialAt(int x, int y) const {
    if (x < 0 || x >= GRID_WIDTH || y < 0 || y >= GRID_HEIGHT) {
        return material_type::DEEP_WATER;
    }
    return grid[y][x];
}

bool isPassable(int x, int y) const {
    material_type mat = getMaterialAt(x, y);
	return materialMap.at(mat).isThisTraversable;
	//return materialMap[mat].isThisTraversable;
}

bool isPassable(const sf::Vector2f& position) const {
    int x = static_cast<int>(position.x) / TILE_SIZE;
    int y = static_cast<int>(position.y) / TILE_SIZE;
    return isPassable(x, y);
}

World() : vertices(sf::Quads, GRID_WIDTH * GRID_HEIGHT * 6) {
	// Initialize the vertex array with the correct size and type
    vertices.setPrimitiveType(sf::Triangles);
    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
	generateWorld();
    
}

private:
void generateWorld() {
    

#pragma warning( push )
#pragma warning( disable : 4244)
    for (int x = 0; x < GRID_WIDTH; x++) {
        for (int y = 0; y < GRID_HEIGHT; y++) {
            
            // Get noise value and color as before
			grid[y].resize(GRID_WIDTH);
            auto m = getColorFromNoise(x, y);
                grid[y][x] = m;
            auto color = materialMap.at(m).color;
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
#pragma warning( pop ) 

}
};