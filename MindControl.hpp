#pragma once
#include <vector>
#include "Ant.h"
#include "Jobs.cpp"
#include "Pheromone.h"

class MindControl {
	std::vector<Job> jcopy;
	sf::Vector2f goHere;
	Ant* controlled_ant;
	Ant::Job mindControlJob;
	Pheromone sp;

	MindControl() {
		// Initialize the mind control system
		controlled_ant = nullptr;
		mindControlJob =  Ant::Job("mind control", pherotype::transport,
			[](Ant& t, float dt, eyes info) {
				
				t.gotoCORD(goHere, dt);
				t.spray()

			},
			[](Ant& t) -> float {


				return 1.0f; // Example: Replace with actual logic  
			}
		),
	}




};