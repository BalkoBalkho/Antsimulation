#include "Simulation.h"
struct simulation siml;

float get_distance(sf::Vector2f a, sf::Vector2f b)
{
    return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

//template<typename T>
// std::vector<T> findNearby(std::list<T> listtogoover, sf::Vector2f pos, float radius)
//{
//	std::vector<T> nearby;
//	for (auto it = listtogoover.begin(); it != listtogoover.end(); ++it) {
//		if (get_distance(it->position, pos) < radius) {
//			nearby.push_back(it);
//			
//		}
//	}
//}
